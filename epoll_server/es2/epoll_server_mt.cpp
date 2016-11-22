#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

#include <string>
#include <list>
#include <mutex>
#include <thread>
#include <fstream>


#define SERVER_PORT 7711
#define SERVER_IP "0.0.0.0"

#define MAX_CONNECT	10
#define INVALID_FD -1
#define INVALID_UID -1

std::mutex gConnMutex; // connection mutex
int epfd = 0; // epoll fd

///// UTIL START [
static int trim(char *str, int max)
{	
	if (max <= 0) {
		max = strlen(str);
	}

	int i;
	for (i=max-1;i>=0;i--) {;
		if (str[i]=='\r' || str[i]=='\n' || str[i]==' ') {
			str[i] = '\0';
			max --;
		} else {
			return max;
		}
	}
	return max;
}

static void read_file(const char *filename, std::string &buffer)
{
	std::ifstream ifile(filename, std::ios::in);	
	if (ifile.is_open() && ifile.good() && !ifile.eof())
	{
		// 1.get total size
		// 2.new same size tmp buffer
		// 3.move seekg to begin
		// 4.read into buffer
		// 5.copy tmp buffer to string buffer
		// 6.delete tmp buffer

		long startPos = ifile.tellg();
		ifile.seekg(0, std::ios::end);
		long endPos = ifile.tellg();

		long len = endPos - startPos;
		char *tmpBuffer = new char[len];
		ifile.seekg(0, std::ios::beg);
		ifile.read(tmpBuffer, len);

		buffer = tmpBuffer;

		delete tmpBuffer;
	}
	ifile.close();
}
///// UTIL END   ]

///// NET UTIL START [

void set_socket_nonblock(int fd)
{
	int opts;
	opts = fcntl(fd, F_GETFL);
	opts = opts | O_NONBLOCK;
	fcntl(fd, F_SETFL, opts);
}

int init_listen_fd(int epfd)
{
	int ret;

	// init listen_fd
	// 1.create normal socket, bind, listen
	// 2.add into epoll, event is EPOLL_IN | EPOLL_ET
	
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	int ip = inet_addr(SERVER_IP);
	sin.sin_addr.s_addr = ip;

	int listen_fd = socket(sin.sin_family, SOCK_STREAM, 0);
	if (listen_fd < 0)
	{
		printf("init_listen_fd:socket fail\n");
		return listen_fd;
	}

	int flag;
	flag = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
	set_socket_nonblock(listen_fd);

	ret = bind(listen_fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
	{
		printf("init_listen_fd:bind fail\n");
		return ret;
	}

	int backlog = 1024;
	ret = listen(listen_fd, backlog);
	if (ret < 0)
	{
		printf("init_listen_fd:listen fail\n");
		return ret;
	}

	/*
	typedef union epoll_data {
		void *ptr;
		int fd;
		__uint32_t u32;
		__uint64_t u64;
	} epoll_data_t;

	struct epoll_event {
		__uint32_t events;  // Epoll events
	    epoll_data_t data;  // User data variable
	};
	*/

	struct epoll_event ev;
	ev.data.fd = listen_fd; // ev.data.fd and ev.data.ptr are union
	ev.events = EPOLLIN | EPOLLET; // EPOLLET: edge triggered
	// ev.events = EPOLLIN;
	// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);

	return listen_fd;
}

///// NET UTIL END   ]

///// CONNECTION UTIL START [

struct connect_t
{
	int fd;
	long uid;
	int events;
	int next_free;
	std::string out_buffer;
	std::mutex mtx;
	void Init()
	{
		fd = -1;
		uid = -1;
		events = 0;
		next_free = 0;
		out_buffer = "";
	}
};

int g_free_connect = 0;
connect_t g_conn_list[MAX_CONNECT];

int init_connection_list()
{
	for (int i=0;i<MAX_CONNECT;i++) {
		g_conn_list[i].Init();
		g_conn_list[i].next_free = i + 1;
	}
	g_conn_list[MAX_CONNECT-1].next_free = -1;
	g_free_connect = 0;
	return 0;
}

connect_t * new_conn(int fd)
{
	gConnMutex.lock();
	static long next_uid = 10;
	connect_t * conn = &g_conn_list[g_free_connect];
	conn->fd = fd;
	conn->uid = next_uid;
	g_free_connect = conn->next_free;
	next_uid ++;
	gConnMutex.unlock();
	return conn;
}

int free_conn(connect_t * conn)
{
	gConnMutex.lock();
	conn->Init();
	conn->next_free = g_free_connect;
	g_free_connect = conn - g_conn_list;
	gConnMutex.unlock();
	return 0;
}

connect_t * get_conn_by_fd(int fd)
{
	gConnMutex.lock();
	connect_t *conn = NULL;
	for (int i = 0; i< MAX_CONNECT; i++)
	{
		if (g_conn_list[i].fd == fd)
		{
			conn = &g_conn_list[i];
			break;
		}
	}
	gConnMutex.unlock();
	return conn;
}

connect_t * get_conn_by_uid(int uid)
{
	gConnMutex.lock();
	connect_t *conn = NULL;
	for (int i = 0; i< MAX_CONNECT; i++)
	{
		if (g_conn_list[i].uid == uid)
		{
			conn = &g_conn_list[i];
			break;
		}
	}
	gConnMutex.unlock();
	return conn;
}

///// CONNECTION UTIL END   ]

///// THREAD UTIL START [

std::list<int> read_uid_list;
std::list<int> write_uid_list;
sem_t read_sem;
sem_t write_sem;
std::mutex read_list_mtx;
std::mutex write_list_mtx;

int pop_uid(std::list<int> &uid_list, std::mutex &mtx)
{
	int uid = -1;
	mtx.lock();
	if (uid_list.size() > 0)
	{
		uid = uid_list.front();
		uid_list.pop_front();
	}
	mtx.unlock();
	return uid;
}

void push_uid(std::list<int> &uid_list, std::mutex &mtx, int uid)
{
	mtx.lock();
	uid_list.push_back(uid);
	mtx.unlock();
}

///// THREAD UTIL END ]

///// FD UTIL START [

void do_read()
{
	// 1.wait for semaphore
	// 2.get uid from uid_list
	// 3.read until EAGAIN
	// 4.if read size == 0, remove from epoll, close fd
	// 5.do send, mod EPOLLOUT in epoll
	
	const int BUFFER_SIZE = 1024000;
	char in_buffer[BUFFER_SIZE+1];
	while (true)
	{
		sem_wait(&read_sem);

		printf("\n---- READ START\n");

		int uid = INVALID_UID;
		uid = pop_uid(read_uid_list, read_list_mtx);
		if (uid == INVALID_UID)
		{
			continue;
		}

		connect_t *conn = get_conn_by_uid(uid);
		if (conn == NULL)
		{
			continue;
		}

		conn->mtx.lock();

		if (conn->uid == uid)
		{

			bool isDisconnect = false;
			int totalSize = 0;
			while (true)
			{
				int size = read(conn->fd, in_buffer, BUFFER_SIZE);
				if (size == 0)
				{
					// EOF, client disconnect
					struct epoll_event c_ev;
					epoll_ctl(epfd, EPOLL_CTL_DEL, conn->fd, &c_ev);
					close(conn->fd);
					free_conn(conn);
					isDisconnect = true;
					printf("read EOF, close socket\n");
					break;
				}

				if (size < 0)
				{
					if (errno == EAGAIN || errno == EWOULDBLOCK)
					{
						// read no more
						printf("read EAGAIN\n");
						break;
					}
					// something go wrong
					printf("*************** read error, errno=%d ***********\n", errno);
					break;
				}

				in_buffer[size] = '\0';
				// TEST add to conn out buffer
				conn->out_buffer.append(in_buffer, size);

				printf("read size=%d\n", size);
				totalSize += size;
			}
			printf("totalSize=%d\n", totalSize);

			if (isDisconnect == false)
			{
				if (totalSize > 0)
				{
					// TEST add EPOLLOUT event
					conn->events = EPOLLIN | EPOLLOUT | EPOLLET;
					struct epoll_event c_ev;
					c_ev.data.fd = conn->fd;
					c_ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_MOD, conn->fd, &c_ev);
				}
			}
		}
		else
		{
			// conn change by other thread, do nothing
		}

		conn->mtx.unlock();

		printf("---- READ END\n\n");
	}
}

void do_write()
{
	// 1.wait for semaphore
	// 2.get uid from uid_list
	// 3.write until EAGAIN
	// 4.if no more data, mod -EPOLLOUT
	while (true)
	{
		sem_wait(&write_sem);
		printf("\n---- WRITE START\n");

		int uid = INVALID_UID;
		uid = pop_uid(write_uid_list, write_list_mtx);
		if (uid == INVALID_UID)
		{
			continue;
		}

		connect_t *conn = get_conn_by_uid(uid);
		if (conn == NULL)
		{
			continue;
		}

		conn->mtx.lock();

		// conn->uid may change by read thread, should check it
		if (conn->uid == uid)
		{

			printf("before write:conn->out_buffer.size()=%lu\n", conn->out_buffer.size());

			if (conn->out_buffer.size() == 0)
			{
				// no more to send, mod as EPOLLIN
				// conn->events = EPOLLIN | EPOLLET;
				conn->events = conn->events & (~EPOLLOUT);
				struct epoll_event c_ev;
				c_ev.data.fd = conn->fd;
				// c_ev.events = EPOLLIN | EPOLLET;
				c_ev.events = conn->events;
				epoll_ctl(epfd, EPOLL_CTL_MOD, conn->fd, &c_ev);
			}
			else
			{
				size_t totalSize = conn->out_buffer.size();
				size_t lastSize = conn->out_buffer.size();
				const char *ptr = conn->out_buffer.c_str();
				while (true)
				{
					/*
					int size = write(conn->fd, conn->out_buffer.c_str(), conn->out_buffer.size());
					if (size < 0)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							// kernel writer buff full, keep EPOLL_OUT event
							printf("write EAGAIN\n");
							break;
						}
						// some thing go wrong
						printf("**************** write error, errno=%d ****************\n", errno);
						break;
					}
					conn->out_buffer = conn->out_buffer.substr(size);
					if (conn->out_buffer.size() == 0)
					{
						break;
					}
					*/
					int size = write(conn->fd, ptr, lastSize);
					if (size < 0)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							// kernel writer buff full, keep EPOLL_OUT event
							printf("write EAGAIN\n");
						}
						else
						{
							// some thing go wrong
							printf("**************** write error, errno=%d ****************\n", errno);
						}
						if (ptr != conn->out_buffer.c_str())
						{
							conn->out_buffer = conn->out_buffer.substr(totalSize-lastSize);
						}
						break;
					}
					ptr += size;
					lastSize -= size;
					if (lastSize == 0)
					{
						conn->out_buffer = "";
						break;
					}
				}

				printf("after write:conn->out_buffer.size()=%lu\n", conn->out_buffer.size());

				if (conn->out_buffer.size() == 0)
				{
					// all out_buffer sended, now should only listen EPOLLIN
					// conn->events = EPOLLIN | EPOLLET;
					conn->events = conn->events & (~EPOLLOUT);
					struct epoll_event c_ev;
					c_ev.data.fd = conn->fd;
					// c_ev.events = EPOLLIN | EPOLLET;
					c_ev.events = conn->events;
					epoll_ctl(epfd, EPOLL_CTL_MOD, conn->fd, &c_ev);
				}
				else
				{
					conn->events = conn->events | EPOLLOUT;
					struct epoll_event c_ev;
					c_ev.data.fd = conn->fd;
					c_ev.events = conn->events;
					epoll_ctl(epfd, EPOLL_CTL_MOD, conn->fd, &c_ev);
				}
			}
		}
		else
		{
			// conn changed by other thread, write thread should do nothing
		}

		conn->mtx.unlock();
		printf("---- WRITE END\n\n");
	}
}

///// FD UTIL END ]

int main()
{
	int ret;

	/*
	const char * filename = "./tmp.txt";
	std::string sendData;
	read_file(filename, sendData); // about 80k
	sendData += sendData; // double it
	sendData += sendData; // double it
	sendData += sendData; // double it
	sendData += sendData; // double it
	sendData += sendData; // double it
	sendData += sendData; // double it
	printf("sendData.size()=%lu\n", sendData.size());
	*/

	// 1.init all connection
	// 2.init epoll
	// 3.init listen_fd
	// 4.add listen_fd into epoll
	// 5.init semaphore
	// 6.init read thread, write thread
	// 7.epoll wait
	
	// 1
	init_connection_list();

	// 2
	// must do epoll_create first
	// int epoll_create(int size); // size is unused nowday
	epfd = epoll_create(20);

	// 3,4
	int listen_fd = init_listen_fd(epfd);
	if (listen_fd < 0)
	{
		return -1;
	}
	printf("listen_fd=%d\n", listen_fd);


	// 5
	sem_init(&read_sem, 0, 0);
	sem_init(&write_sem, 0, 0);

	// 6
	std::thread read_thread(do_read);
	std::thread write_thread(do_write);

	printf("EPOLLIN=%d EPOLLOUT=%d\n", EPOLLIN, EPOLLOUT);

	const int MAX_EVENTS = 20;
	struct epoll_event events[MAX_EVENTS];

	while (true)
	{
		// int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
		int nfds = epoll_wait(epfd, events, MAX_EVENTS, 10000);
		printf("nfds = %d\n", nfds);
		for (int i=0;i<nfds;i++)
		{
			printf("events[i].events=%d\n", events[i].events);

			if (events[i].data.fd == listen_fd)
			{
				printf("\n---- ACCEPT START\n");
				// listen_fd set EPOLL_ET, accept until EAGAIN
				while (true)
				{
					// accept client
					// 1.accept
					// 2.set nonblock, tcp nodealy
					// 3.add into epoll, event is EPOLLIN | EPOLLET
					struct sockaddr_in c_sin;
					bzero(&c_sin, sizeof(c_sin));
					socklen_t sin_size = sizeof(c_sin);
					int client_fd = accept(listen_fd, (struct sockaddr*)&c_sin, &sin_size);	
					if (client_fd < 0)
					{
						if (errno == EAGAIN || EWOULDBLOCK)
						{
							// no more connection
							break;
						}
						else
						{
							// accept error, some thing to wrong
							printf("accept error, errno=%d\n", errno);
							break;
						}
					}
					printf("accept:new_conn %d\n", client_fd);

					set_socket_nonblock(client_fd);

					int flag = 1;
					ret = setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
					
					connect_t * conn = new_conn(client_fd);

					conn->events = EPOLLIN;

					struct epoll_event c_ev;
					c_ev.data.fd = conn->fd;
					c_ev.events = EPOLLIN | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &c_ev);
				}

				printf("---- ACCEPT END\n\n");
				continue;
			}

			// read client
			if (events[i].events & EPOLLIN)
			{
				printf("---- EPOLLIN EVENT START----\n");

				int fd = events[i].data.fd;
				printf("fd=%d\n", fd);

				connect_t *conn  = get_conn_by_fd(fd);
				if (conn == NULL)
				{
					perror("EPOLLIN conn null");
					continue;
				}

				conn->mtx.lock();

				// remove EPOLLIN listen
				conn->events = conn->events & (~EPOLLIN);
				struct epoll_event c_ev;
				c_ev.data.fd = conn->fd;
				c_ev.events = conn->events;
				epoll_ctl(epfd, EPOLL_CTL_MOD, conn->fd, &c_ev);

				conn->mtx.unlock();

				// add into read list
				push_uid(read_uid_list, read_list_mtx, conn->uid);
				sem_post(&read_sem);

				printf("---- EPOLLIN EVENT END----\n\n");
			}

			// send client
			if (events[i].events & EPOLLOUT)
			{
				printf("---- EPOLLOUT EVENT START----\n");

				int fd = events[i].data.fd;
				printf("fd=%d\n", fd);

				connect_t *conn  = get_conn_by_fd(fd);
				if (conn == NULL)
				{
					perror("EPOLLOUT conn null");
					continue;
				}

				conn->mtx.lock();

				// remove EPOLLIN listen
				conn->events = conn->events & (~EPOLLOUT);
				struct epoll_event c_ev;
				c_ev.data.fd = conn->fd;
				c_ev.events = conn->events;
				epoll_ctl(epfd, EPOLL_CTL_MOD, conn->fd, &c_ev);

				conn->mtx.unlock();

				// add into write list	
				push_uid(write_uid_list, write_list_mtx, conn->uid);
				sem_post(&write_sem);
				printf("---- EPOLLOUT EVENT END----\n\n");
			}
		}
	}
	
	return 0;
		
}

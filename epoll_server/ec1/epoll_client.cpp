#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <string>
#include <fstream>
#include <list>


#define SERVER_PORT 7711
#define SERVER_IP "0.0.0.0"

struct timeval start_time;
struct timeval end_time;
int SEND_SIZE = 0;
std::list<double> timeList;

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

static void write_file(const char *filename, const char *data, int size)
{
	std::ofstream ofile(filename, std::ios::out | std::ios::ate | std::ios::app);	
	if (ofile.is_open() && ofile.good())
	{
		ofile.write(data, size);
	}
	ofile.close();
}

void calculate_avg_time()
{
	double total = 0;
	double avg_time = 0;
	for (auto iter = timeList.begin(); iter != timeList.end(); iter++)
	{
		total += *iter;
	}
	avg_time = total / timeList.size();
	printf("num=%zu avg_time=%lf\n", timeList.size(), avg_time);
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

int init_client_fd(int epfd)
{
	int ret;

	// init client_fd
	// 1.create normal socket
	// 2.connect
	// 3.add into epoll, event is EPOLL_IN | EPOLL_ET
	
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	int ip = inet_addr(SERVER_IP);
	sin.sin_addr.s_addr = ip;

	int client_fd = socket(sin.sin_family, SOCK_STREAM, 0);
	if (client_fd < 0)
	{
		printf("init_client_fd:socket fail\n");
		return client_fd;
	}

	ret = connect(client_fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
	{
		printf("init_client_fd:connect fail errno=%d\n", errno);
		return ret;
	}
	set_socket_nonblock(client_fd);

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
	ev.data.fd = client_fd; // ev.data.fd and ev.data.ptr are union
	ev.events = EPOLLIN | EPOLLET; // EPOLLET: edge triggered
	// ev.events = EPOLLIN;
	// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
	epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);

	return client_fd;
}

///// NET UTIL END   ]

int main()
{
	int ret;
	ret = 0;

	const char * filename = "./tmp.txt";
	std::string sendData;
	read_file(filename, sendData); // about 80k
	sendData += sendData; // double it
	sendData += sendData; // double it
	sendData += sendData; // double it
	sendData += sendData; // double it
	sendData += sendData; // double it
	sendData += sendData; // double it
	/*
	*/
	printf("sendData.size()=%lu\n", sendData.size());
	SEND_SIZE = sendData.size();
	int totalSize = 0;

	/*
	const char * filename2 = "./tmp2.txt";
	const char * filedata = "hello world\n";
	write_file(filename2, filedata, strlen(filedata));
	*/

	// 1.init epoll
	// 2.init client_fd
	// 3.add client_fd into epoll
	// 4.add stdin fd into epoll
	// 5.epoll wait

	const int MAX_EVENTS = 20;
	struct epoll_event events[MAX_EVENTS];	

	// must do epoll_create first
	// int epoll_create(int size); // size is unused nowday
	int epfd = 0;
	epfd = epoll_create(1);

	int client_fd = init_client_fd(epfd);
	if (client_fd < 0)
	{
		return -1;
	}
	printf("client_fd=%d\n", client_fd);
	std::string out_buffer = "";

	int stdinfd = fileno(stdin);
	set_socket_nonblock(stdinfd);
	struct epoll_event ev;
	ev.data.fd = stdinfd; // ev.data.fd and ev.data.ptr are union
	ev.events = EPOLLIN | EPOLLET; // EPOLLET: edge triggered
	epoll_ctl(epfd, EPOLL_CTL_ADD, stdinfd, &ev);

	while (true)
	{
		// int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
		int nfds = epoll_wait(epfd, events, MAX_EVENTS, 10000);
		printf("nfds = %d\n", nfds);
		for (int i=0;i<nfds;i++)
		{
			printf("events[i].events=%d\n", events[i].events);

			// stdin
			if (events[i].data.fd == stdinfd)
			{
				const int BUFFER_SIZE = 10240;
				char in_buffer[BUFFER_SIZE];
				bzero(in_buffer, BUFFER_SIZE);

				// stdinfd set EPOLL_ET, accept until EAGAIN
				while (true)
				{
					int size = read(stdinfd, in_buffer, BUFFER_SIZE);
					if (size == 0)
					{
						// EOF
						break;
					}

					if (size < 0)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							// read no more
							break;
						}
						// something go wrong
						printf("*************** stdin read error, errno=%d ***********\n", errno);
						break;
					}

					// trim(in_buffer, strlen(in_buffer));
					printf("in_buffer=[%s]\n", in_buffer);
					// out_buffer += in_buffer;
				}

				totalSize = 0;
				// test, send big data to client
				out_buffer += sendData;
				
				struct epoll_event c_ev;
				c_ev.data.fd = client_fd;
				c_ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_MOD, client_fd, &c_ev);


				// mark start_time
				gettimeofday(&start_time, NULL);

				continue;
			}

			// client read
			if (events[i].events&EPOLLIN)
			{
				// 1.read fd, read until EAGAIN
				// 2.if read size == 0, remove from epoll, close fd
				// 3.do send, mod EPOLLOUT in epoll
				printf("\n---- READ START\n");
				const int BUFFER_SIZE = 1024000;
				char in_buffer[BUFFER_SIZE+1];
				bzero(in_buffer, BUFFER_SIZE+1);

				while (true)
				{
					int size = read(client_fd, in_buffer, BUFFER_SIZE);
					if (size == 0)
					{
						// EOF, server disconnect
						epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, &events[i]);
						close(client_fd);
						printf("read EOF, close socket\n");
						calculate_avg_time();
						return 0;
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

					in_buffer[size] = 0;
					// printf("in_buffer=[%s]\n", in_buffer);
					// const char * filename2 = "./tmp2.txt";
					// write_file(filename2, in_buffer, size);
					printf("read size=%d\n", size);
					totalSize += size;
				}
				printf("totalSize = %d\n", totalSize);

				if (totalSize == SEND_SIZE)
				{
					gettimeofday(&end_time, NULL);
					double totalTime = end_time.tv_sec * 1000 + double(end_time.tv_usec) / 1000 - start_time.tv_sec * 1000 - double(start_time.tv_usec) / 1000;
					printf("total time=%lfms\n", totalTime);
					timeList.push_back(totalTime);
				}

				printf("---- READ END\n\n");
			}

			// client write
			if (events[i].events&EPOLLOUT)
			{
				printf("\n---- WRITE START\n");

				printf("before write:out_buffer.size()=%lu\n", out_buffer.size());

				if (out_buffer.size() == 0)
				{
					// no more to send, mod as EPOLLIN
					struct epoll_event c_ev;
					c_ev.data.fd = client_fd;
					c_ev.events = EPOLLIN | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_MOD, client_fd, &c_ev);
					continue;
				}

				while (true)
				{
					int size = write(client_fd, out_buffer.c_str(), out_buffer.size());
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

					if ((size_t)size == out_buffer.size())
					{
						out_buffer = "";
					}
					else
					{
						out_buffer = out_buffer.substr(size);
					}

					printf("out_buffer.size()=%lu\n", out_buffer.size());
					if (out_buffer.size() == 0)
					{
						break;
					}
				}

				printf("after write:out_buffer.size()=%lu\n", out_buffer.size());

				if (out_buffer.size() == 0)
				{
					// all out_buffer sended, now should only listen EPOLLIN
					struct epoll_event c_ev;
					c_ev.data.fd = client_fd;
					c_ev.events = EPOLLIN | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_MOD, client_fd, &c_ev);
				}

				printf("---- WRITE END\n\n");
			}
		}
	}
	
	return 0;
		
}

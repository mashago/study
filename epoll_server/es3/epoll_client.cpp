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

#include "msg_head.h"


#define SERVER_PORT 7711
#define SERVER_IP "0.0.0.0"

#define BUFFER_SIZE 10240

int SEND_SIZE = 0;

char g_in_buffer[BUFFER_SIZE+1];
char *g_in_start_pos = g_in_buffer;
char *g_in_end_pos = g_in_buffer;
char g_out_buffer[BUFFER_SIZE+1];
char *g_out_start_pos = g_out_buffer; // pos to send start
char *g_out_end_pos = g_out_buffer; // pos to send end

uint32_t g_msg_index = 1;

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

///// UTIL END   ]

///// NET UTIL START [

void set_fd_nonblock(int fd)
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

	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0)
	{
		printf("init_client_fd:socket fail\n");
		return client_fd;
	}
	
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	int ip = inet_addr(SERVER_IP);
	sin.sin_addr.s_addr = ip;

	ret = connect(client_fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
	{
		printf("init_client_fd:connect fail errno=%d\n", errno);
		return ret;
	}
	set_fd_nonblock(client_fd);

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
	ev.events = EPOLLIN;
	// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
	epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);

	return client_fd;
}

int read_msg(char *in_buffer, int size)
{
	if (size < msg_head_t::MSG_HEAD_LEN)
	{
		// not include a full msg head
		return 0;
	}
	msg_head_t header;
	char *ptr = header.unpack(in_buffer);
	printf("read_msg:cmd_id=%d body_len=%d msg_index=%u\n", header.cmd_id, header.body_len, header.msg_index);
	if (size - msg_head_t::MSG_HEAD_LEN < header.body_len)
	{
		return 0;
	}

	char body[BUFFER_SIZE];
	memcpy(body, ptr, header.body_len);
	body[header.body_len] = '\0';
	printf("read_msg:body=%s\n", body);
	
	return msg_head_t::MSG_HEAD_LEN + header.body_len;
}

int do_read(int client_fd, int epfd)
{
	// 1.read from socket
	// 2.try unpack msg
	
	int ret = 0;
	int size = read(client_fd, g_in_end_pos, BUFFER_SIZE - (g_in_end_pos - g_in_buffer));
	if (size == 0)
	{
		// EOF, server disconnect
		epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, NULL);
		close(client_fd);
		printf("read EOF, close socket\n");
		exit(0);
	}

	if (size < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// read no more
			printf("read EAGAIN\n");
			return 0;
		}
		// something go wrong
		printf("*************** read error, errno=%d ***********\n", errno);
		exit(0);
	}
	g_in_end_pos += size;

	int total_size = g_in_end_pos - g_in_start_pos;
	while (g_in_start_pos != g_in_end_pos)
	{
		ret = read_msg(g_in_start_pos, total_size);
		if (ret == 0)
		{
			// no more msg
			break;
		}
		
		g_in_start_pos += ret;
		total_size -= ret;
	}

	if (total_size == 0)
	{
		g_in_start_pos = g_in_buffer;
		g_in_end_pos = g_in_buffer;
	}

	return 0;
}

int do_write(int client_fd, int epfd)
{
	if (g_out_end_pos == g_out_buffer)
	{
		// no more to send, mod as EPOLLIN
		struct epoll_event c_ev;
		c_ev.data.fd = client_fd;
		c_ev.events = EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_MOD, client_fd, &c_ev);
		return 0;
	}

	int size = write(client_fd, g_out_start_pos, g_out_end_pos - g_out_start_pos);
	if (size < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// kernel writer buff full, keep EPOLL_OUT event
			printf("write EAGAIN\n");
			return 0;
		}
		// some thing go wrong
		printf("**************** write error, errno=%d ****************\n", errno);
		exit(0);
	}
	g_out_start_pos += size;

	if (g_out_end_pos == g_out_start_pos)
	{
		printf("all buffer sended\n");
		g_out_start_pos = g_out_buffer;
		g_out_end_pos = g_out_buffer;

		// all out_buffer sended, now should only listen EPOLLIN
		struct epoll_event c_ev;
		c_ev.data.fd = client_fd;
		c_ev.events = EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_MOD, client_fd, &c_ev);
	}

	return 0;
}

///// NET UTIL END   ]

int main()
{
	int ret;
	ret = 0;

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
	SEND_SIZE = sendData.size();
	int totalSize = 0;
	*/

	// 1.init epoll
	// 2.init client_fd, add into epoll
	// 3.add stdin fd into epoll
	// 4.epoll wait

	// 1.
	// must do epoll_create first
	// int epoll_create(int size); // size is unused nowday
	int epfd = 0;
	epfd = epoll_create(1);
	if (epfd < 0)
	{
		printf("epoll_create errno %d\n", errno);
		return -1;
	}

	// 2.
	int client_fd = init_client_fd(epfd);
	if (client_fd < 0)
	{
		printf("init_client_fd errno %d\n", errno);
		return -1;
	}
	printf("client_fd=%d\n", client_fd);

	// 3.
	int stdinfd = fileno(stdin);
	set_fd_nonblock(stdinfd);
	struct epoll_event ev;
	ev.data.fd = stdinfd; // ev.data.fd and ev.data.ptr are union
	ev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, stdinfd, &ev);

	// 4.
	const int MAX_EVENTS = 20;
	struct epoll_event events[MAX_EVENTS];	
	while (true)
	{
		// int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
		int nfds = epoll_wait(epfd, events, MAX_EVENTS, 10000);
		printf("nfds = %d\n", nfds);
		for (int i = 0;i < nfds; i++)
		{
			// read stdin
			if (events[i].data.fd == stdinfd)
			{
				// 1.read from stdin
				// 2.create msg header
				// 3.add header and body into send buffer
				// 4.epoll ctl

				// 1.
				char tmp_buffer[BUFFER_SIZE+1];
				int size = read(stdinfd, tmp_buffer, BUFFER_SIZE);
				if (size == 0)
				{
					// EOF
					continue;
				}
				if (size < 0)
				{
					printf("read stdinfd error, errno=%d\n", errno);
					exit(0);
				}

				// 2.
				if (BUFFER_SIZE - (g_out_end_pos - g_out_buffer) < msg_head_t::MSG_HEAD_LEN + size)
				{
					// no space to send
					printf("warn no space to send\n");
					continue;
				}

				msg_head_t header;
				header.cmd_id = 1;
				header.body_len = size;
				header.msg_index = g_msg_index;
				g_msg_index++;

				// 3.
				char *ptr = header.pack(g_out_end_pos);
				memcpy(ptr, tmp_buffer, size);
				g_out_end_pos += (msg_head_t::MSG_HEAD_LEN + size);

				// 4.
				struct epoll_event c_ev;
				c_ev.data.fd = client_fd;
				c_ev.events = EPOLLIN | EPOLLOUT;
				epoll_ctl(epfd, EPOLL_CTL_MOD, client_fd, &c_ev);

				// XXX try send again
				/*
				{
					if (BUFFER_SIZE - (g_out_end_pos - g_out_buffer) < msg_head_t::MSG_HEAD_LEN + size)
					{
						// no space to send
						printf("warn no space to send\n");
						continue;
					}

					msg_head_t header;
					header.cmd_id = 1;
					header.body_len = size;
					header.msg_index = g_msg_index;
					g_msg_index++;

					char *ptr = header.pack(g_out_end_pos);
					memcpy(ptr, tmp_buffer, size);
					g_out_end_pos += (msg_head_t::MSG_HEAD_LEN + size);
				}
				*/

				continue;
			}

			// client read
			if (events[i].events&EPOLLIN)
			{
				do_read(client_fd, epfd);
			}

			// client write
			if (events[i].events&EPOLLOUT)
			{
				do_write(client_fd, epfd);
			}
		}
	}
	
	return 0;
		
}

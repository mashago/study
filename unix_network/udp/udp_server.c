#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

#include "data.h"

#define SERVER_IP	"0.0.0.0"
#define SERVER_PORT 7777

#define MAX_CLIENT 10

#define BUFFER_SIZE 1000

/*
 * select io: 
 * 1.init fd_sets, fd_set readset, fd_set writeset, FD_ZERO(&readset), FDZERO(&writeset)
 * 2.FD_SET(fd, &fdset) // add listen
 * 3.FD_CLR(fd, &fdset) // delete listen
 * 4.num_ready = select(int nfds, &readset, &writeset, &exceptset, &timeval); // core logic
 * 5.FD_ISSET(fd, &fdset) // check fd if is ready
 */

typedef struct {
	int status;
	struct sockaddr_in sin;
} client_t;

client_t g_client_list[MAX_CLIENT];

int do_broadcast(int fd)
{
	int ret;
	int len = sizeof(struct sockaddr_in);
	char buffer[BUFFER_SIZE];
	sprintf(buffer, "%ld", time(NULL));
	for (int i=0; i<MAX_CLIENT; i++)
	{
		if (g_client_list[i].status == 0)
		{
			continue;
		}

		ret = sendto(fd, buffer, strlen(buffer), 0, (struct sockaddr *)&(g_client_list[i].sin), len);
		if (ret == 0) {
			printf("sendto:error %d\n", errno);
			g_client_list[i].status = 0;		
			continue;
		}
		if (ret < 0) {
			printf("sendto:error %d\n", errno);
			g_client_list[i].status = 0;		
			continue;
		}

	}
	return 0;
}

int add_client(struct sockaddr_in client)
{
	for (int i=0; i<MAX_CLIENT; i++)
	{
		client_t * ptr = g_client_list + i;
		if (ptr->status == 0)
		{
			continue;
		}
		if (ptr->sin.sin_addr.s_addr == client.sin_addr.s_addr
		&& ptr->sin.sin_port == client.sin_port)
		{
			return 0;
		}
	}

	for (int i=0; i<MAX_CLIENT; i++)
	{
		client_t * ptr = g_client_list + i;
		if (ptr->status == 0)
		{
			ptr->sin = client;
			ptr->status = 1;
			return 0;
		}
	}

	return -1;
}

int main(int argc, char *argv[])
{
	int ret;

	bzero(g_client_list, sizeof(g_client_list));

	// server socket
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	inet_aton(SERVER_IP, &sin.sin_addr);
	sin.sin_port = htons(SERVER_PORT);

	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd == -1)
	{
		printf("socket errno=%d\n", errno);
		exit(1);
	}

	ret = bind(fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0) {
		printf("bind errno=%d\n", errno);
		exit(1);
	}

	struct sockaddr_in clients[MAX_CLIENT];
	bzero(clients, sizeof(clients));
	
	int num_ready;

	int nfds;
	fd_set rset; // save read fds
	FD_ZERO(&rset); // must empty fd_set

	struct timeval timeout;

	while (1) {
		FD_SET(fd, &rset);	 // listen stdin fd

		bzero(&timeout, sizeof(timeout)); // must bzero it
		timeout.tv_sec = 100;
		// The first nfds descriptors are checked in each set; i.e., the descriptors from 0 through nfds-1 in the descriptor sets are examined.  (Example: If you have set two file descriptors "4" and "17", nfds should  not be "2", but rather "17 + 1" or "18".)
		nfds = fd+1;

		// int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval * timosut);
		num_ready = select(nfds, &rset, NULL, NULL, &timeout);
		printf("select:num_ready=%d\n", num_ready);
		if (num_ready < 0) {
			printf("select:errno=%d\n", errno);
		}

		// do broadcast
		// do_broadcast(fd);

		if (FD_ISSET(fd, &rset)) {
			printf("----socket_fd\n");
			
			struct sockaddr_in client;
			socklen_t len = sizeof(client);

			// do recv
			Message msg;
			int size = 0;
			size = recvfrom(fd, (char *)&msg, sizeof(Message), 0, (struct sockaddr *)&client, &len);
			if (size == -1)
			{
				printf("recvfrom error=%d\n", errno);
				continue;
			}
			printf("client: [%s]:[%d]\n", inet_ntoa(client.sin_addr), client.sin_port);
			printf("recv msg time=%ld status=%d data=%s index=%ld\n", msg.time, msg.status, msg.data, msg.index);

			// add_client(client);

			msg.status = 2;
			// do send
			size = sendto(fd, (char *)&msg, sizeof(Message), 0, (struct sockaddr *)&client, len);
			if (size < 0) {
				printf("sendto:error %d\n", errno);
				continue;
			}
		}

	}

	return 0;
}

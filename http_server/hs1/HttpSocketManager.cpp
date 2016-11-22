#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include "HttpSocketManager.h"

HttpSocketManager * HttpSocketManager::m_pThis = NULL;

HttpSocketManager::HttpSocketManager()
{
}

HttpSocketManager::~HttpSocketManager()
{
}

int HttpSocketManager::Init(std::string addr, int port)
{
	int ret;
	main_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (main_fd < 0) {
		printf("socket:errno=%d\n", errno);
		return -1;
	}

	int flag = 1;
	setsockopt(main_fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&flag, sizeof(flag));

	bzero(&sin, sizeof(sin));

	sin.sin_family = AF_INET;
	// printf("inet_addr=%u\n", inet_addr(SERVER_ADDR));
	// sin.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	inet_aton(addr.c_str(), &sin.sin_addr);
	sin.sin_port = htons(port);

	char tmp_buff[100];
	inet_ntop(sin.sin_family, &(sin.sin_addr), tmp_buff, sizeof(sin)); // get ip
	int ppp = ntohs(sin.sin_port);
	sprintf(tmp_buff+strlen(tmp_buff), ":%d", ppp);
	printf("address=%s\n", tmp_buff);

	ret = bind(main_fd, (struct sockaddr*)&sin, sizeof(sin));
	if (ret < 0) {
		printf("bind:errno=%d\n", errno);
		return -1;
	}

	ret = listen(main_fd, 1024);
	if (ret < 0) {
		printf("listen:errno=%d\n", errno);
		return -1;
	}

	return 0;
}

int HttpSocketManager::Wait(int timeout)
{
	struct sockaddr_in cin;
	bzero(&cin, sizeof(cin));
	socklen_t len = sizeof(cin);
	client_fd = accept(main_fd, (struct sockaddr*)&cin, &len); 
	return 1;
}

int HttpSocketManager::Recv(char *buff, int maxSize)
{
	int ret;
	ret = read(client_fd, buff, maxSize);
	if (ret <= 0) {
		close(client_fd);	
		return -1;
	}
	buff[ret] = '\0';
	return 0;
}

int HttpSocketManager::Send(const char *buff, int size)
{
	write(client_fd,buff,size);
	return 0;
}

int HttpSocketManager::Disconnect()
{
	close(client_fd);
	return 0;
}


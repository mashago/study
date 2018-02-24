
#include <stdio.h>
#include <errno.h>
#include <zmq.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define TRY_FORK 1

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	void *pCtx = NULL;
	void *pSocket = NULL;

	const char *endpoint = "ipc://server.ipc";

	if ((pCtx = zmq_ctx_new()) == NULL)
	{
		printf("pCtx NULL\n");
		return -1;
	}
	printf("ctx\n");
	getchar();

	if ((pSocket = zmq_socket(pCtx, ZMQ_XREQ)) == NULL)
	{
		printf("pSocket NULL\n");
		zmq_ctx_destroy(pCtx);
		return -1;
	}
	printf("socket\n");
	getchar();

	if (zmq_connect(pSocket, endpoint) != 0)
	{
		printf("bind fail %d\n", errno);
		zmq_close(pSocket);
		zmq_ctx_destroy(pCtx);
		return -1;
	}
	printf("connect [%s]\n", endpoint);
	getchar();
	
	const int buff_size = 1000;
	char buff[buff_size+1] = {0};

	int num = 0;

#ifdef TRY_FORK
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		printf("fork error\n");
		exit(1);
	}
	printf("socket\n");
	getchar();
#else
	int pid = 0;
#endif

#ifdef TRY_FORK
	if (pid != 0)
	{
		sleep(1);
	}
#endif

	while (1)
	{
		++num;
		snprintf(buff, buff_size, "client send buff[%d][%d]", pid, num);
		int len = zmq_send(pSocket, buff, strlen(buff), 0);
		if (len < 0)
		{
			printf("pid=%d errno=%d error=%s\n", pid, errno, zmq_strerror(errno));
			sleep(2);
			continue;
		}
		printf("pid=%d len=%d\n", pid, len);

		sleep(2);
	}
	
	zmq_close(pSocket);
	zmq_ctx_destroy(pCtx);

	return 0;
}

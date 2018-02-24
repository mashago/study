
#include <stdio.h>
#include <errno.h>
#include <zmq.h>
#include <stdlib.h>
#include <unistd.h>

// #define TRY_FORK 1
// #define TRY_MUTI 1

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

	if ((pSocket = zmq_socket(pCtx, ZMQ_XREQ)) == NULL)
	{
		printf("pSocket NULL\n");
		zmq_ctx_destroy(pCtx);
		return -1;
	}
	printf("socket\n");

	if (zmq_bind(pSocket, endpoint) != 0)
	{
		printf("bind fail %d\n", errno);
		zmq_close(pSocket);
		zmq_ctx_destroy(pCtx);
		return -1;
	}
	printf("bind [%s]\n", endpoint);

	const int buff_size = 1000;

#ifdef TRY_FORK
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		printf("fork error\n");
		exit(1);
	}
	printf("fork\n");
#else
	int pid = 0;
#endif

#ifdef TRY_MUTI
	void *pCtx2 = NULL;
	void *pSocket2 = NULL;

	if ((pCtx2 = zmq_ctx_new()) == NULL)
	{
		printf("pCtx2 NULL\n");
		return -1;
	}
	printf("ctx2\n");
	getchar();

	if ((pSocket2 = zmq_socket(pCtx2, ZMQ_XREQ)) == NULL)
	{
		printf("pSocket2 NULL\n");
		zmq_ctx_destroy(pCtx2);
		return -1;
	}
	printf("socket2\n");
	getchar();
#endif

	while (0)
	{
		char buff[buff_size] = {0};
		int len = zmq_recv(pSocket, buff, buff_size, 0);
		if (len < 0)
		{
			printf("pid=%d errno=%d error=%s\n", pid, errno, zmq_strerror(errno));
			sleep(2);
			continue;
		}
		printf("pid=%d len=%d buff=%s\n", pid, len, buff);
	}

	zmq_close(pSocket);
	printf("close socket\n");
	getchar();

	zmq_ctx_destroy(pCtx);
	printf("ctx destory\n");
	getchar();


	return 0;
}


#include <stdio.h>
#include <errno.h>
#include <zmq.h>

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

	if ((pSocket = zmq_socket(pCtx, ZMQ_XREQ)) == NULL)
	{
		printf("pSocket NULL\n");
		zmq_ctx_destroy(pCtx);
		return -1;
	}

	if (zmq_bind(pSocket, endpoint) != 0)
	{
		printf("bind fail %d\n", errno);
		zmq_close(pSocket);
		zmq_ctx_destroy(pCtx);
		return -1;
	}
	printf("bind [%s]\n", endpoint);
	
	const int buff_size = 1000;
	char buff[buff_size] = {0};
	while (1)
	{
		int len = zmq_recv(pSocket, buff, buff_size, 0);
		if (len < 0)
		{
			printf("error=%s\n", zmq_strerror(errno));
			continue;
		}
		printf("len=%d buff=%s\n", len, buff);
	}
	
	zmq_close(pSocket);
	zmq_ctx_destroy(pCtx);

	return 0;
}

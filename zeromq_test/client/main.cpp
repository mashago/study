
#include <stdio.h>
#include <errno.h>
#include <zmq.h>
#include <string.h>
#include <unistd.h>

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

	if (zmq_connect(pSocket, endpoint) != 0)
	{
		printf("bind fail %d\n", errno);
		zmq_close(pSocket);
		zmq_ctx_destroy(pCtx);
		return -1;
	}
	printf("bind [%s]\n", endpoint);
	
	const int buff_size = 1000;
	char buff[buff_size] = {0};
	int num = 0;
	while (1)
	{
		++num;
		snprintf(buff, buff_size, "client send buff[%d]", num);
		int len = zmq_send(pSocket, buff, strlen(buff), 0);
		if (len < 0)
		{
			printf("error=%s\n", zmq_strerror(errno));
			continue;
		}
		printf("len=%d\n", len);

		sleep(2);
	}
	
	zmq_close(pSocket);
	zmq_ctx_destroy(pCtx);

	return 0;
}

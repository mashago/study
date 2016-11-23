/*
 * System V message queue
 */
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h> // system v msg queue
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MQ_PATH	"/tmp"
#define MQ_ID	2

#define MQ_KEY	60038 // to 0x0000ea86
// #define MQ_OFLAG	(IPC_CREAT | IPC_EXCL) // system v flag
#define MQ_OFLAG	(IPC_CREAT) // system v flag

/*
 *
 *
 */

#define BUFFER_SIZE 100
typedef struct {
	long mtype;
	char mchar[BUFFER_SIZE+1];
} Message;

#define MAX_MESSAGE_SIZE (sizeof(Message) - sizeof(long))

int main(int argc, char **argv)
{
	int ret;
	printf("---- msg queue server ----\n");

	// get key for message queue
	/*
	key_t key = ftok(MQ_PATH, MQ_ID);
	if (key == -1)
	{
		printf("ftok errno=%d\n", errno);
		exit(1);
	}
	*/
	key_t key = (key_t)MQ_KEY;
	printf("key=%d\n", key);

	int msqid = msgget(key, MQ_OFLAG | 0664);
	if (msqid == -1)
	{
		printf("msgget errno=%d\n", errno);
		exit(1);
	}
	printf("msqid=%d\n", msqid);

	while (1)
	{

		Message m1;
		m1.mtype = 1;

		fgets(m1.mchar, BUFFER_SIZE, stdin);
		// strncpy(m1.mchar, "hello mq1", BUFFER_SIZE);

		// flag = 0 or IPC_NOWAIT
		msgsnd(msqid, (void *)&m1, sizeof(Message)-sizeof(long), 0);

	}

	return 0;
};

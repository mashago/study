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
#define MQ_ID	1

#define MQ_KEY	60037 // to 0x0000ea85
// #define MQ_OFLAG	(IPC_CREAT | IPC_EXCL) // system v flag
#define MQ_OFLAG	(IPC_CREAT) // system v flag

/*
 *
 *
 */

#define BUFFER_SIZE 100
typedef struct {
	long mtype;
	char mtext[BUFFER_SIZE+1];
} Message;

#define MAX_MESSAGE_SIZE (sizeof(Message) - sizeof(long))

int main(int argc, char **argv)
{
	int ret;
	printf("system v msg queue\n");

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

	/*
	int msqid = msgget(key, MQ_OFLAG | 0664);
	if (msqid == -1)
	{
		printf("msgget errno=%d\n", errno);
		exit(1);
	}
	printf("msqid=%d\n", msqid);
	*/

	pid_t pid;
	if ((pid = fork()) == 0)
	{
		// child
		int msqid = msgget(key, MQ_OFLAG | 0664);
		if (msqid == -1)
		{
			printf("child:msgget errno=%d\n", errno);
			exit(1);
		}
		printf("child:msqid=%d\n", msqid);

		Message msg;
		printf("sizeof(Message)=%u\n", sizeof(Message));
		printf("sizeof(msg)=%u\n", sizeof(msg));
		printf("sizeof(msg.mtype)=%u\n", sizeof(msg.mtype));
		printf("sizeof(msg.mtext)=%u\n", sizeof(msg.mtext));
		bzero(msg.mtext, sizeof(msg.mtext));

		// int msgsz = 9;
		int msgsz = MAX_MESSAGE_SIZE;
		printf("child:msgsz=%d\n", msgsz);

		// flag = 0 or IPC_NOWAIT
		ssize_t size = msgrcv(msqid, (void *)&msg, msgsz, 0, 0);
		if (size < 0)
		{
			printf("child:errno=%d\n", errno);
		}
		else
		{
			printf("child:size=%zd msg.mtype=%ld msg.mtext=%s\n", size, msg.mtype, msg.mtext);
		}
		exit(0);
	}

	// parent

	int msqid = msgget(key, MQ_OFLAG | 0664);
	if (msqid == -1)
	{
		printf("parent:msgget errno=%d\n", errno);
		exit(1);
	}

	printf("parent:msqid=%d\n", msqid);

	Message msg;
	msg.mtype = time(NULL);

	const char *ptr = "1234567890";
	sprintf(msg.mtext, "%s", ptr);

	int msgsz = strlen(msg.mtext);
	msgsz = msgsz / 2;
	// msgsz = 0; // only send mtype...
	printf("parent:msg.mtype=%lu msg.mtext=%s msgsz=%d\n", msg.mtype, msg.mtext, msgsz);

	// flag = 0 or IPC_NOWAIT
	// msgsz should be mtext send size
	msgsnd(msqid, (void *)&msg, msgsz, 0);

	sleep(2);

	msgctl(msqid, IPC_RMID, NULL);
	
	waitpid(pid, NULL, 0);

	return 0;
};

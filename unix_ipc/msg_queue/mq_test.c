#ifdef __linux__
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>

#define MQ_KEY 40001 // 0x9C41
#define MQ_OFLAG (IPC_CREAT)

#define BUFFER_SIZE 1000
typedef struct
{
	long mtype;
	char mtext[BUFFER_SIZE+1];
} Message;

int mqid;

void * thread_func(void *param)
{
	long type = *((long *)param);

	Message msg;
	int msgflg = 0; // 0 will block, IPC_NOWAIT will non-block
	ssize_t size = msgrcv(mqid, (void *)&msg, sizeof(msg.mtext), type, msgflg); // recv by type
	if (size < 0)
	{
		perror("thread_func:msgrcv");
	}
	else
	{
		msg.mtext[size] = '\0';
		printf("size=%zd msg.mtype=%ld msg.mtext=%s\n", size, msg.mtype, msg.mtext);
	}

	return NULL;
}

void print_mq_stat(int mqid, int index)
{
	struct msqid_ds mqds;
	if (msgctl(mqid, IPC_STAT, &mqds) == -1)
	{
		perror("main:msgctl IPC_STAT");
		exit(0);
	}

	// struct msqid_ds {
	//    struct ipc_perm msg_perm;     /* Ownership and permissions */
	//    time_t          msg_stime;    /* Time of last msgsnd(2) */
	//    time_t          msg_rtime;    /* Time of last msgrcv(2) */
	//    time_t          msg_ctime;    /* Time of last change */
	//    unsigned long   __msg_cbytes; /* Current number of bytes in
	// 									queue (non-standard) */
	//    msgqnum_t       msg_qnum;     /* Current number of messages
	// 									in queue */
	//    msglen_t        msg_qbytes;   /* Maximum number of bytes
	// 									allowed in queue */
	//    pid_t           msg_lspid;    /* PID of last msgsnd(2) */
	//    pid_t           msg_lrpid;    /* PID of last msgrcv(2) */
	// };

	printf("%d: stat __msg_cbypes=%lu msg_qnum=%lu msg_qbytes=%lu\n", index, mqds.__msg_cbytes, mqds.msg_qnum, mqds.msg_qbytes);
}

void set_mq_stat(int mqid)
{
	if (getuid() != 0)
	{
		// only root can update msg_qbytes beyond MSGMNB
		return;
	}

	struct msqid_ds mqds;
	if (msgctl(mqid, IPC_STAT, &mqds) == -1)
	{
		perror("main:msgctl IPC_STAT");
		exit(0);
	}

	// struct msqid_ds {
	//    struct ipc_perm msg_perm;     /* Ownership and permissions */
	//    time_t          msg_stime;    /* Time of last msgsnd(2) */
	//    time_t          msg_rtime;    /* Time of last msgrcv(2) */
	//    time_t          msg_ctime;    /* Time of last change */
	//    unsigned long   __msg_cbytes; /* Current number of bytes in
	// 									queue (non-standard) */
	//    msgqnum_t       msg_qnum;     /* Current number of messages
	// 									in queue */
	//    msglen_t        msg_qbytes;   /* Maximum number of bytes
	// 									allowed in queue */
	//    pid_t           msg_lspid;    /* PID of last msgsnd(2) */
	//    pid_t           msg_lrpid;    /* PID of last msgrcv(2) */
	// };

	// update max bytes in queue
	// only root can update msg_qbytes beyond MSGMNB
	mqds.msg_qbytes = 70000;
	// mqds.msg_qbytes = 4000;
	
	if (msgctl(mqid, IPC_SET, &mqds) == -1)
	{
		perror("main:msgctl IPC_SET");
		exit(0);
	}
}

void print_mq_info(int mqid, int index)
{
	struct msginfo mqinfo;
	if (msgctl(mqid, IPC_INFO, (struct msqid_ds *)&mqinfo) == -1)
	{
		perror("main:msgctl IPC_INFO");
		exit(0);
	}

	// struct msginfo {
	//   int msgpool; /* Size in kibibytes of buffer pool
	// 				  used to hold message data;
	// 				  unused within kernel */
	//   int msgmap;  /* Maximum number of entries in message
	// 				  map; unused within kernel */
	//   int msgmax;  /* Maximum number of bytes that can be
	// 				  written in a single message */
	//   int msgmnb;  /* Maximum number of bytes that can be
	// 				  written to queue; used to initialize
	// 				  msg_qbytes during queue creation
	// 				  (msgget(2)) */
	//   int msgmni;  /* Maximum number of message queues */
	//   int msgssz;  /* Message segment size;
	// 				  unused within kernel */
	//   int msgtql;  /* Maximum number of messages on all queues
	// 				  in system; unused within kernel */
	//   unsigned short int msgseg;
	// 			   /* Maximum number of segments;
	// 				  unused within kernel */
	// };

	printf("%d: info msgmax=%d msgmnb=%d msgmni=%d\n", index, mqinfo.msgmax, mqinfo.msgmnb, mqinfo.msgmni);
}

void whoami()  
{  
	printf("uid=%d euid=%d gid=%d\n", getuid(), geteuid(), getgid());  
} 

int main(int argc, char **argv)
{
	printf("hello mq_test\n");

	whoami();

	// create msg queue
	mqid = msgget(MQ_KEY, MQ_OFLAG | 0664);
	if (mqid < 0)
	{
		perror("main:msgget");
		exit(0);
	}

	print_mq_stat(mqid, 1);
	print_mq_info(mqid, 1);
	set_mq_stat(mqid);

	// send msg
	Message msg;
	long type1 = 1;
	long type2 = 2;

	msg.mtype = type1;
	sprintf(msg.mtext, "12345");
	msgsnd(mqid, (void *)&msg, strlen(msg.mtext), 0);
	printf("msgsnd1\n");

	msg.mtype = type2;
	sprintf(msg.mtext, "qwert");
	msgsnd(mqid, (void *)&msg, strlen(msg.mtext), 0);
	printf("msgsnd2\n");

	print_mq_stat(mqid, 2);

	sleep(1);

	// recv thread
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, thread_func, (void *)&type1);
	pthread_create(&tid2, NULL, thread_func, (void *)&type2);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	print_mq_stat(mqid, 3);

	msgctl(mqid, IPC_RMID, NULL);

	return 0;
}

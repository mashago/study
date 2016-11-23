/*
 * POSIX semaphore test
 */ 
#include <stdio.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#ifdef __linux__
#define SEM_PATH	"semfile.1"
#else
#define SEM_PATH	"/tmp/semfile.1"
#endif

#define SEM_FLAG	(O_CREAT | O_EXCL)
#define FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
 *
 * unname semaphore
 * int sem_init(sem_t *sem, int pshared, unsigned int value);
 * int sem_destroy(sem_t *sem);
 *
 * int sem_wait(sem_t *sem);
 * int sem_post(sem_t *sem);
 *
 *
 */

sem_t g_sem;

void * thread_func(void * param)
{
	sem_t *psem = (sem_t *)param;	
	printf("child sem_wait() start\n");
	sem_wait(psem);
	printf("child sem_wait() finish\n");
	return NULL;
}

int main(int argc, char **argv)
{
	int ret;
	printf("semaphore_test\n");


	const int DEFAULT_VALUE = 0;
	ret = sem_init(&g_sem, 0, DEFAULT_VALUE);
	if (ret != 0)
	{
		printf("sem_init:errno=%d\n", errno);
		exit(0);
	}

#ifdef __linux__
	int value;
	ret = sem_getvalue(&g_sem, &value);
	if (ret != 0) {
		printf("sem_getvalue:errno=%d\n", errno);
		exit(0);
	}
	printf("g_sem value=%d\n", value);
#endif


	pthread_t tid;
	pthread_create(&tid, NULL, thread_func, (void *)&g_sem);

	sleep(1);	
	sem_post(&g_sem);

	sleep(3);	

	sem_destroy(&g_sem);

	return 0;
};

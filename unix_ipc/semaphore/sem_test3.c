/*
 * POSIX semaphore test
 */ 
#include <stdio.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

/*
 *
 * unname semaphore
 * int sem_init(sem_t *sem, int pshared, unsigned int value);
 * int sem_destroy(sem_t *sem);
 *
 * int sem_wait(sem_t *sem);
 *
 * int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
 * struct timespec {
 * 		time_t tv_sec;      
 * 		long   tv_nsec;     
 * };
 *
 * int sem_post(sem_t *sem);
 *
 *
 */

int thread_running = 1;
sem_t g_sem;

void * thread_func(void * param)
{
	while (thread_running)
	{
		sem_t *psem = (sem_t *)param;	

		printf("child sem_wait() start\n");
		// sem_wait(psem);
		#ifdef __linux__
		struct timeval tv;
		gettimeofday(&tv, NULL);
		struct timespec ts;
		ts.tv_sec = tv.tv_sec;
		ts.tv_nsec = tv.tv_usec * 1000;
		ts.tv_sec += 3; // 3 sec later from now

		int ret = sem_timedwait(psem, &ts);
		#else
		// apple osx not support sem_timedwait
		int ret = sem_wait(psem);
		#endif
		if (ret != 0)
		{
			if (errno == ETIMEDOUT)
			{
				printf("sem_timedwait:time out\n");
			}
			if (errno == EINVAL)
			{
				printf("sem_timedwait:time invalid\n");
			}
		}
		printf("child sem_wait() finish\n\n");
	}
	printf("thread end\n");
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

	const int MAX_BUFFER = 100;
	char buffer[MAX_BUFFER];
	while (1)
	{
		if (fgets(buffer, MAX_BUFFER, stdin))
		{
			if (buffer[0] == 'q')
			{
				thread_running = 0;
				sem_post(&g_sem);
				break;
			}

			sem_post(&g_sem);
			continue;
		}
		thread_running = 0;
		sem_post(&g_sem);
		break;
	}

	pthread_join(tid, NULL);

	sem_destroy(&g_sem);

	return 0;
};

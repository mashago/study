#ifdef __linux__
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;
int t1 = 1;
int t2 = 2;

void mutex_func()
{
	printf("before lock ----\n");
	pthread_mutex_lock(&mutex);
	printf("after lock ----\n");

	sleep(1);

	printf("before unlock ----\n");
	pthread_mutex_unlock(&mutex);
	printf("after unlock ----\n");
}

void * thread_func(void *param)
{
	int num = *((int *)param);
	printf("thread_func num=%d ****\n", num);

	printf("before lock num=%d ****\n", num);
	pthread_mutex_lock(&mutex);
	printf("after lock num=%d ****\n", num);

	sleep(1);
	mutex_func();

	printf("before unlock num=%d ****\n", num);
	pthread_mutex_unlock(&mutex);
	printf("after unlock num=%d ****\n", num);

	return NULL;
}

int main(int argc, char **argv)
{
	// 1.init mutex attr
	// 2.init mutex
	// 3.create pthread

	// 1
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); // set mutex can recursive
	// pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP); // set mutex can recursive
	
	printf("PTHREAD_MUTEX_RECURSIVE=%d PTHREAD_MUTEX_RECURSIVE_NP=%d\n", PTHREAD_MUTEX_RECURSIVE, PTHREAD_MUTEX_RECURSIVE_NP);

	// 2
	pthread_mutex_init(&mutex, &attr);

	// 3
	pthread_t tid1;
	pthread_t tid2;

	pthread_create(&tid1, NULL, thread_func, (void *)&t1);
	pthread_create(&tid2, NULL, thread_func, (void *)&t2);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}

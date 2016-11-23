#ifdef __linux__
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>

// a test for pthread_mutex_t in shared memory, and lock between process
// http://blog.chinaunix.net/uid-26651253-id-3342091.html

#define SHARE_ACCESS (PROT_READ | PROT_WRITE)
#define SHARE_FLAG (MAP_SHARED | MAP_ANON)

pthread_mutex_t * create_mutex()
{
	// 1.create pthread_mutex_t in shared memory
	// 2.init pthread_mutex_t
	
	pthread_mutex_t *mutex = mmap(NULL, sizeof(pthread_mutex_t), SHARE_ACCESS, SHARE_FLAG, -1, 0);
	if (mutex == MAP_FAILED)
	{
		perror("create_mutex:mmap");
		return NULL;
	}

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED); // must set this, default is PTHREAD_PROCESS_PRIVATE. if use default setting, will cause undefine case

	pthread_mutex_init(mutex, &attr);

	return mutex;
}

// use local mutex, check if work
pthread_mutex_t * create_mutex2()
{
	// 1.create pthread_mutex_t in shared memory
	// 2.init pthread_mutex_t
	
	pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
	if (mutex == NULL)
	{
		perror("create_mutex2:malloc");
		return NULL;
	}

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

	pthread_mutex_init(mutex, &attr);

	return mutex;
}

int main(int argc, char **argv)
{
	printf("hello pthread_test2\n");

	pthread_mutex_t *mutex = create_mutex();
	if (mutex == NULL)
	{
		exit(1);
	}

	pid_t pid = fork();
	if (pid < 0)
	{
		perror("main:fork");
		exit(1);
	}
	if (pid == 0)
	{
		// child process
		printf("child:before lock ----\n");
		pthread_mutex_lock(mutex);
		printf("child:after lock ----\n");
		sleep(1);
		printf("child:before unlock ----\n");
		pthread_mutex_unlock(mutex);
		printf("child:after unlock ----\n");
		return 0;
	}

	// parent process

	printf("parent:before lock ****\n");
	pthread_mutex_lock(mutex);
	printf("parent:after lock ****\n");
	sleep(1);
	printf("parent:before unlock ****\n");
	pthread_mutex_unlock(mutex);
	printf("parent:after unlock ****\n");

	sleep(3);

	munmap(mutex, sizeof(pthread_mutex_t));

	return 0;
}

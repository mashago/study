#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
pthread_cond_t cond;

void * func1(void *arg)
{
	int *count = (int*) arg;
	printf("func1:before_lock\n");
	pthread_mutex_lock (&lock);
	printf("func1:before_cond_wait \n");
	// block, wait cond signal, mutex unlock
	pthread_cond_wait( &cond, &lock);
	printf("func1:after_cond_wait \n");
	*count = *count -1;
	printf("func1:count=%d\n", *count);
	pthread_mutex_unlock (&lock);
	printf("func1:after_unlock\n");
	return arg;
}

void * func2(void *arg)
{
	int *count = (int*) arg;
	printf("func2:before_lock\n");
	pthread_mutex_lock(&lock);
	printf("func2:after_lock\n");
	printf("func2:before_cond_signal\n");
	// send free cond wait signal, only one cond-wait-pthread can get this signal
	pthread_cond_signal(&cond);
	printf("func2:after_cond_signal\n");
	*count=*count+2;
	printf("func2:count=%d\n", *count);
	pthread_mutex_unlock(&lock);
	printf("func2:after_unlock\n");
	return arg;
}

void * func3(void *arg)
{
	int *count = (int*) arg;
	printf("func3:before_lock\n");
	pthread_mutex_lock(&lock);
	printf("func3:count=%d\n", *count);
	pthread_mutex_unlock(&lock);
	printf("func3:after_unlock\n");
	return arg;
}

void * func4(void *arg)
{
	int *count = (int*) arg;
	printf("func4:before_lock\n");
	pthread_mutex_lock (&lock);
	printf("func4:before_cond_wait \n");
	pthread_cond_wait( &cond, &lock);
	printf("func4:count=%d\n", *count);
	pthread_mutex_unlock (&lock);
	printf("func4:after_unlock\n");
	return arg;
}

int main(void)
{
	pthread_t tid1,tid2,tid3;
	pthread_t tid4;
	int count = 0;

	// init mutex lock
	pthread_mutex_init(&lock, NULL);
	// init pthread condition
	pthread_cond_init(&cond, NULL);

	pthread_create(&tid1, NULL, func1, &count);
	pthread_create(&tid4, NULL, func4, &count);

	
	sleep(1);

	pthread_create(&tid2 ,NULL, func2, &count);

	pthread_create(&tid3, NULL, func3, &count);

	sleep(4);

	// let main thread join to pthread tid1 and main thread will block here wait tid1 return
	// void *status;
	// pthread_join(tid1, &status);

	printf("main:count=%d\n", count);
}

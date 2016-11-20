#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

pthread_mutex_t g_mutex;

int g_a = 0;
int g_b = 0;
int g_c = 0;

double get_time_ms()
{
	struct timeval tv;    
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

void * use_mutex(void *arg)
{
	double time1 = get_time_ms();

	int count = *((int *)arg);
	while (count > 0)
	{
		pthread_mutex_lock(&g_mutex);
		g_a++;
		pthread_mutex_unlock(&g_mutex);
		count--;
	}

	double time2 = get_time_ms();

	printf("use_mutex:time=%lfms\n", time2-time1);

	return NULL;
}

void * use_sync(void *arg)
{
	double time1 = get_time_ms();

	int count = *((int *)arg);
	while (count > 0)
	{
		__sync_fetch_and_add(&g_b, 1);
		count--;
	}

	double time2 = get_time_ms();

	printf("use_sync:time=%lfms\n", time2-time1);
	return NULL;
}

void * no_lock(void *arg)
{
	double time1 = get_time_ms();

	int count = *((int *)arg);
	while (count > 0)
	{
		g_c++;
		count--;
	}

	double time2 = get_time_ms();

	printf("no_lock:time=%lfms\n", time2-time1);
	return NULL;
}

int main(int argc, char **argv)
{
	printf("hello locker\n");

	pthread_mutex_init(&g_mutex, NULL);

	int count = 10000;

	{
		pthread_t t1, t2, t3;
		pthread_create(&t1, NULL, use_mutex, &count);
		pthread_create(&t2, NULL, use_mutex, &count);
		pthread_create(&t3, NULL, use_mutex, &count);
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		pthread_join(t3, NULL);

		printf("g_a=%d\n\n", g_a);
	}

	{
		pthread_t t1, t2, t3;
		pthread_create(&t1, NULL, use_sync, &count);
		pthread_create(&t2, NULL, use_sync, &count);
		pthread_create(&t3, NULL, use_sync, &count);
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		pthread_join(t3, NULL);

		printf("g_b=%d\n\n", g_b);
	}

	{
		pthread_t t1, t2, t3;
		pthread_create(&t1, NULL, no_lock, &count);
		pthread_create(&t2, NULL, no_lock, &count);
		pthread_create(&t3, NULL, no_lock, &count);
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		pthread_join(t3, NULL);

		printf("g_c=%d\n\n", g_c);
	}

	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>


/*
 * gprof test
 * $(CXX) -pg ....
 * ./a.out
 * gprof a.out gmon.out > report.txt
 */

struct itimerval g_itimer;

void print_hello()
{
	printf("call print_hello\n");

	double x = 0;
	for (int i = 0; i < 1000000000; ++i)
	{
		x += i;
	}

}

void print_hello2()
{
	printf("call print_hello2\n");

	double x = 0;
	for (int i = 0; i < 1000000000; ++i)
	{
		x += i;
	}

}

void * print_hello3(void *arg)
{
	setitimer(ITIMER_PROF, &g_itimer, NULL);
	printf("call print_hello3\n");

	double x = 0;
	for (int i = 0; i < 10000000; ++i)
	{
		x += i;
	}

	return NULL;
}

int test0()
{
	print_hello();
	return 0;
}

int test1()
{
	print_hello();
	print_hello2();
	return 0;
}

int test2()
{
	getitimer(ITIMER_PROF, &g_itimer);
	pthread_t t1;
	pthread_create(&t1, NULL, print_hello3, (void *)NULL);

	print_hello();
	pthread_join(t1, NULL);
	return 0;
}

int test_notyet() 
{
	// int ret;

	// ret = 0;
	return 0;
}

typedef int (*testcase_t) ();
testcase_t test_list[] =
{
	test0
,	test1
,	test2
};

int main(int argc, char *argv[]) 
{
	int ret;
	int maxcase;
	int testcase;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;

	if (argc > 1) {
		if (!strcmp(argv[1], "all"))
		{
			printf("run all case\n");
			for (int i=0; i<maxcase; i++)
			{
				printf("run case[%d]\n", i);
				ret = test_list[i]();
				if (ret != 0) 
				{
					printf("case[%d] ret=%d\n", i, ret);
					return 0;
				}
			}
			return 0;
		}
		testcase = atoi(argv[1]);
		if (testcase < 0 || testcase >= maxcase) 
		{
			testcase = maxcase - 1;
		}
	}

	printf("run case[%d]\n", testcase);
	ret = test_list[testcase]();
	if (ret != 0) 
	{
		printf("case[%d] ret=%d\n", testcase, ret);
	}

	return 0;
}


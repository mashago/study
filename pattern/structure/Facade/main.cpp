
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// high level interface which use some low level inerface inside

class SubSystemA
{
public:
	void Operation()
	{
	}
};

class SubSystemB
{
public:
	void Operation()
	{
	}
};

class SubSystemC
{
public:
	void Operation()
	{
	}
};

class Facade
{
public:
	void Operation()
	{
		SubSystemA a;
		SubSystemB b;
		SubSystemC c;

		a.Operation();
		b.Operation();
		c.Operation();
	}
};

int test0()
{
	Facade f;
	f.Operation();

	return 0;
}

int test1()
{
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
// ,	test1
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


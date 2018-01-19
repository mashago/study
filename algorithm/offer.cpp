
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test0()
{
	return 0;
}

bool duplicate(int numbers[], int length, int *duplication)
{
	if (numbers == nullptr || length <= 0)
	{
		return false;
	}

	for (int i=0; i<length; ++i)
	{
		if (numbers[i] < 0 || numbers[i] > length - 1)
		{
			return false;
		}
	}

	// core: make i in pos numbers[i]
	for (int i=0; i<length; ++i)
	{
		while (numbers[i] != i)
		{
			if (numbers[i] == numbers[numbers[i]])
			{
				*duplication = numbers[i];
				return true;
			}
			// swap
			int temp = numbers[i];
			numbers[i] = numbers[temp];
			numbers[temp] = temp;
		}
	}
	return false;
}

int test1()
{
	int out = -1;
	bool ret = false;
	int numbers[] = {0, 1, 2, 3, 3};
	ret = duplicate(numbers, sizeof(numbers)/sizeof(int), &out);
	printf("ret=%d out=%d\n", ret, out);


	return 0;
}


int countRange(const int *numbers, int length, int start, int end);
int getDuplication(const int *numbers, int length)
{
	if (numbers == nullptr || length <= 0)
	{
		return -1;
	}
	int start = 1;
	int end = length - 1;

	while (end >= start)
	{
		int middle = ((end - start) >> 1) + start;
		int count = countRange(numbers, length, start, middle);
		if (end == start)
		{
			if (count > 1)
				return start;
			else
				break;
		}

		if (count > (middle - start + 1))
			end = middle;
		else
			start = middle + 1;
	}
	return -1;
}

int countRange(const int *numbers, int length, int start, int end)
{
	if (numbers == nullptr)
		return 0;
	
	int count = 0;
	for (int i = 0; i < length; ++i)
	{
		if (numbers[i] >= start && numbers[i] <= end)
			++count;
	}
	return count;
}

int test2() 
{
	int out = -1;
	int numbers[] = {1, 2, 3, 3};
	out = getDuplication(numbers, sizeof(numbers)/sizeof(int));
	printf("out=%d\n", out);
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


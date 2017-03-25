
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void * my_memmove(void *s1, const void *s2, size_t n)
{
	char *sc1;
	const char *sc2;

	sc1 = (char *)s1;
	sc2 = (const char *)s2;

	if (sc1 > sc2 && sc2 + n > sc1)
	{
		// copy backward
		// sc2 --- sc1 --- (sc2 + n) --- (sc1 + n)
		for (sc1 += n, sc2 += n; n > 0; --n)
		{
			*--sc1 = *--sc2;
		}
	}
	else
	{
		// copy forward
		// sc1 --- sc2 --- (sc1 + n) --- (sc2 + n)
		for (; n > 0; --n)
		{
			*sc1++ = *sc2++;
		}
	}

	return s1;
}

int test0()
{
	{
		char buff[] = "1234567";
		printf("buff=%s\n", buff);

		my_memmove(buff, buff+1, 3);
		printf("buff=%s\n", buff);
	}
	{
		char buff[] = "1234567";
		printf("buff=%s\n", buff);

		my_memmove(buff+1, buff, 3);
		printf("buff=%s\n", buff);
	}
	return 0;
}

void * my_memcpy(void *s1, const void *s2, size_t n)
{
	char *sc1;
	const char *sc2;

	for (sc1 = s1, sc2 = s2; n > 0; --n)
	{
		*sc1++ = *sc2++;
	}

	return s1;
}

int test1()
{
	{
		const char *in = "qwerty";
		char buff[] = "1234567";
		printf("buff=%s in=%s\n", buff, in);

		my_memcpy(buff, in, 3);
		printf("buff=%s\n", buff);
	}
	return 0;
}

char * my_strcpy(char *s1, const char *s2)
{
	char *s = s1;

	while (*s++ = *s2++);

	return s1;
}

int test2()
{
	{
		const char *in = "qwerty";
		char buff[] = "1234567";
		printf("buff=%s in=%s\n", buff, in);

		my_strcpy(buff, in);
		printf("buff=%s\n", buff);
	}
	return 0;
}

char * my_strncpy(char *s1, const char *s2, size_t n)
{
	char *s = s1;

	for (; (*s++ = *s2++) && n > 0; n--);

	for (; n > 0; n--)
	{
		*s++ = '\0';
	}

	return s1;
}

void print_to_ascii(const char *s, size_t n)
{
	for (; n > 0; n--)
	{
		printf("%d ", *s++);
	}
	printf("\n");
}

int test3()
{
	{
		const char *in = "abcdefg";
		char buff[] = "123456789";

		size_t size = sizeof(buff) / sizeof(char);
		printf("size=%d\n", size);

		printf("buff=%s in=%s\n", buff, in);
		print_to_ascii(buff, size);
		print_to_ascii(in, strlen(in) + 1);

		my_strncpy(buff, in, size);
		printf("buff=%s\n", buff);
		print_to_ascii(buff, size);
	}
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
,	test3
};

int main(int argc, char *argv[]) 
{
	int ret;
	int maxcase;
	int testcase;

	maxcase = sizeof(test_list) / sizeof(testcase_t);
	testcase = maxcase - 1;

	if (argc > 1)
	{
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


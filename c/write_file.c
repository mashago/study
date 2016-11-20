#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int test0()
{
	// open and close file everytime
	int is_running = 1;
	const char *filename = "out.log";
	const int MAX_BUFFER = 100;
	char in_buffer[MAX_BUFFER];

	while (is_running)
	{
		int ret = read(0, in_buffer, MAX_BUFFER);
		if (ret < 0)
		{
			printf("test0:read fail errno=%d\n", errno);
			return -1;
		}

		if (ret == 0)
		{
			continue;
		}
		in_buffer[ret] = '\0';
		printf("test0:read ret=%d in_buffer=[%s]\n", ret, in_buffer);

		FILE *pfile = fopen(filename, "a");
		if (pfile == NULL)
		{
			printf("test0:fopen fail errno=%d\n", errno);
			return -1;
		}

		ret = fwrite(in_buffer, ret, 1, pfile);
		if (ret < 0)
		{
			printf("test0:fwrite fail errno=%d\n", errno);
			return -1;
		}
		printf("test0:fwrite ret=%d\n", ret);

		fclose(pfile);
	}

	return 0;
}

int test1()
{
	// open file onetime on start
	int is_running = 1;
	const char *filename = "out.log";
	const int MAX_BUFFER = 100;
	char in_buffer[MAX_BUFFER];

	FILE *pfile = fopen(filename, "a");
	if (pfile == NULL)
	{
		printf("test1:fopen fail errno=%d\n", errno);
		return -1;
	}

	while (is_running)
	{
		int ret = read(0, in_buffer, MAX_BUFFER);
		if (ret < 0)
		{
			printf("test1:read fail errno=%d\n", errno);
			return -1;
		}

		if (ret == 0)
		{
			continue;
		}
		in_buffer[ret] = '\0';
		printf("test1:read ret=%d in_buffer=[%s]\n", ret, in_buffer);

		if (strcmp(in_buffer, "q\n") == 0)
		{
			break;
		}

		ret = fwrite(in_buffer, ret, 1, pfile);
		if (ret < 0)
		{
			printf("test1:fwrite fail errno=%d\n", errno);
			return -1;
		}
		printf("test1:fwrite ret=%d\n", ret);

		ret = fflush(pfile);
		if (ret < 0)
		{
			printf("test1:fflush fail errno=%d\n", errno);
			return -1;
		}
	}
	if (pfile != NULL)
	{
		fclose(pfile);
	}

	return 0;
}

int main(int argc, char **argv)
{
	printf("hello write_file\n");

	// test0();
	test1();
	return 0;
}

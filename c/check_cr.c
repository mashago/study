#include <stdio.h>
#include <stdlib.h>

void check_cr(const char *file)
{
	FILE *pfile;
	if ((pfile = fopen(file, "r")) == NULL)
	{
		printf("open file[%s] fail\n", file);
		return;
	}

	long len = 0;
	fseek(pfile, 0, SEEK_END);
	len = ftell(pfile);
	char *buffer = (char *)malloc(len);
	rewind(pfile);

	fread(buffer, sizeof(char), len, pfile);

	int line = 0;
	for (int i = 0; i < len; i++)
	{
		if (buffer[i] != '\n')
		{
			continue;
		}
		line++;
		if (i - 1 < 0 || buffer[i-1] != '\r')
		{
			printf("line[%d] miss \\r\n", line);
		}
	}
	printf("total miss cr line=%d\n", line);

	free(buffer);
}

int main(int argc, char **argv)
{
	printf("hello check_cr\n");

	if (argc < 2)
	{
		printf("need file\n");
		return 0;
	}

	check_cr(argv[1]);
		
	return 0;
}

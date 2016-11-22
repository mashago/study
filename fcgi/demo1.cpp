
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcgi_stdio.h>
#include <iostream>
#include <string>

#define MAX_CONTENT_LENGTH 4096 // 4k

void print_env(const char *key)
{
		// or use re-defined printf
		FCGI_printf("<br>%s: <i>%s</i><br>\r\n"
		, key, getenv(key));
}

int main(int argc, char **argv)
{
	int count = 0;

	char content[MAX_CONTENT_LENGTH+1];
	bzero(content, sizeof(content));

	while (FCGI_Accept() >= 0)
	{
		// NOTE: must has Content-type
		FCGI_printf("Content-type: text/html\r\n" 
		"\r\n" 
		"<title>FastCGI Hello!</title>" 
		"<h1>FastCGI Hello!</h1>" 
		"Request number %d<br>\r\n"
		, ++count);

		FCGI_printf("<br>argc=%d<br>\r\n", argc);
		for (int i=0; i<argc; i++)
		{
			FCGI_printf("<br>argv[%d]=%s<br>\r\n", i, argv[i]);
		}

		print_env("SERVER_NAME");

		print_env("SERVER_ADDR");

		print_env("SERVER_PORT");

		print_env("REMOTE_ADDR");

		print_env("REMOTE_PORT");

		print_env("SCRIPT_FILENAME");

		print_env("QUERY_STRING");

		print_env("REQUEST_METHOD");

		print_env("CONTENT_TYPE");

		print_env("CONTENT_LENGTH");

		print_env("SCRIPT_NAME");

		print_env("REQUEST_URI");

		print_env("DOCUMENT_URI");

		int content_length = atoi(getenv("CONTENT_LENGTH"));

		if (content_length > MAX_CONTENT_LENGTH)
		{
			FCGI_printf("<br>CONTENT_LENGTH over size, content_length=%d max=%d<br>\r\n", content_length, MAX_CONTENT_LENGTH);
			continue;
		}

		if (content_length > 0)
		{
			int ret = FCGI_fread(content, content_length, 1, FCGI_stdin);
			if (ret != 1)
			{
				// something go wrong
				FCGI_printf("<br>FCGI_fread error %d<br>\r\n", ret);
				continue;
			}
			content[content_length] = '\0';

			FCGI_printf("<br>CONTENT: <i>%s</i><br>\n", content);
		}

	}
	return 0;
};

#ifndef _HTTP_UTIL_H_
#define _HTTP_UTIL_H_

#include <stdio.h>

/*
 * perror and exit
 */
inline void error_exit(const char *str)
{
	perror(str);
	exit(1);
}

/*
 * recv one line data from client, size is the max buffer size
 */
inline int get_line(int fd, char *buffer, int size)
{
	// 1.recv char one by one, add into buffer
	// 2.'\0' terminate
	// 3.return data size
	
	int i = 0;
	char c = '\0';
	int n;

	while ((i < size - 1) && (c != '\n'))
	{
		n = recv(fd, &c, 1, 0);
		if (n > 0)
		{
			if (c == '\r')
			{
				// after '\r' should be a '\n', if not, end it
				n = recv(fd, &c, 1, MSG_PEEK);
				if ((n > 0) && (c == '\n'))
				{
					recv(fd, &c, 1, 0);
				}
				else
				{
					c = '\n';
				}
			}
			buffer[i] = c;
			i++;
		}
		else
		{
			c = '\n';
		}
	}
	buffer[i] = '\0';

	return i;
}

#endif

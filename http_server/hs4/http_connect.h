#ifndef _HTTP_CONNECT_H_
#define _HTTP_CONNECT_H_


extern "C"
{
#include <time.h>
#include <pthread.h>
}
#include <string>

#define MAX_CONNECT_BUFFER 10240

// a node for connection list
struct http_connect_t
{
	int fd;
	int64_t uid;
	int next_free; // next free connect index
	time_t active_time;

	char in_buffer[MAX_CONNECT_BUFFER+1];
	char *in_start_pos; // last handle pos
	char *in_end_pos; // last recv pos

	std::string out_buffer;
	int out_pos; // last send pos

	pthread_mutexattr_t lock_attr;
	pthread_mutex_t lock;

	http_connect_t()
	{
		fd = -1;
		uid = -1;
		next_free = -1;
		active_time = 0;
		in_start_pos = in_buffer;
		in_end_pos = in_buffer;
		out_buffer = "";
		out_pos = 0;
		pthread_mutexattr_init(&lock_attr);
		pthread_mutexattr_settype(&lock_attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&lock, &lock_attr);
	}

	void init()
	{
		fd = -1;
		uid = -1;
		next_free = -1;
		active_time = 0;
		in_start_pos = in_buffer;
		in_end_pos = in_buffer;
		out_buffer = "";
		out_pos = 0;
	}

	// refresh active_time
	void active()
	{
		active_time = time(NULL);
	}

	int is_valid()
	{
		if (fd < 0)
		{
			return 0;
		}

		if (uid < 0)
		{
			return 0;
		}

		return 1;
	}
};

#endif

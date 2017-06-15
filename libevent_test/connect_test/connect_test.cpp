/**
 project base on libevent2.0
  **/
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>

#include <event2/event.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
}

// #define SERVER_HOST "0.0.0.0"
#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 7711

#define bzero(ptr, len) memset(ptr, 0, len)

double g_connect_time = 0;
bool g_is_connecting = false;
bool g_is_connected = false;
struct bufferevent *g_bev = NULL;

void timer_cb(evutil_socket_t fd, short event, void *user_data);
void event_cb(struct bufferevent *bev, short what, void *user_data);
void do_connect(struct event_base *main_event);

void timer_cb(evutil_socket_t fd, short event, void *user_data)
{
	if (event != EV_TIMEOUT)
	{
		printf("timer_cb fd=%d event=%d\n", fd, event);
	}

	if (errno != 0)
	{
		printf("timer_cb errno=%d\n", errno);
		errno = 0;
	}

	struct event_base *main_event = (struct event_base *)user_data;
	if (g_is_connected)
	{
		printf("already connected\n");
		return;
	}

	if (g_is_connecting)
	{
		// printf("!!!!!!!!!!! connecting\n");
		return;
	}

	// do connect
	do_connect(main_event);
}

void event_cb(struct bufferevent *bev, short what, void *user_data)
{
	if (errno != 0)
	{
		printf("event_cb errno=%d\n", errno);
		errno = 0;
	}
	evutil_socket_t fd = bufferevent_getfd(bev);
	// struct event_base *main_event = (struct event_base *)user_data;
	
	if (what & BEV_EVENT_READING  
	|| what & BEV_EVENT_WRITING  
	|| what & BEV_EVENT_EOF      
	|| what & BEV_EVENT_ERROR)
	{
		printf("******* free connect fd=%d what=%d\n", fd, what);
		{
			struct timeval tv;
			gettimeofday(&tv, NULL);
			printf("total connect time = %lf\n", tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0 - g_connect_time);
			g_connect_time = 0;
		}
		g_is_connecting = false;
		g_is_connected = false;
		bufferevent_free(g_bev);
		g_bev = NULL;

	}
	else if (what & BEV_EVENT_TIMEOUT)
	{
		printf("event_cb: timeout fd=%d\n", fd);
	}
	else if (what & BEV_EVENT_CONNECTED)
	{
		printf("event_cb: event connected fd=%d\n", fd);
		g_is_connected = true;
	}
	else
	{
		printf("event_cb: other event fd=%d\n", fd);
	}
}

void do_connect(struct event_base *main_event)
{
	static int counter = 1;
	printf("------- do_connect counter=%d\n", counter++);
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = PF_INET;
	sin.sin_addr.s_addr = inet_addr(SERVER_HOST);
	sin.sin_port = htons(SERVER_PORT);

	struct bufferevent *bev = bufferevent_socket_new(main_event, -1, BEV_OPT_CLOSE_ON_FREE);
	if (!bev)
	{
		printf("do_connect: bufferevent connect fail\n");
		return;
	}
	if (errno != 0)
	{
		printf("do_connect errno1=%d\n", errno);
		errno = 0;
	}

	bufferevent_setcb(bev, NULL/*read_cb*/, NULL, event_cb, main_event);
	bufferevent_enable(bev, EV_READ);

	int ret = bufferevent_socket_connect(bev, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
	{
		printf("do_connect: bufferevent connect fail\n");
		bufferevent_free(bev);
		return;
	}
	if (errno != 0)
	{
		printf("do_connect errno2=%d\n", errno);
		errno = 0;
	}

	g_bev = bev;
	g_is_connecting = true;

	struct timeval tv;    
	gettimeofday(&tv,NULL);
	g_connect_time = tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	struct event_base *main_event = event_base_new();
	if (main_event == NULL)
	{
		printf("event_base_new fail\n");
		return 0;
	}

	{
		struct event *timer_event = event_new(main_event, -1, EV_PERSIST, timer_cb, (void *)main_event);
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 10 * 1000;
		if (event_add(timer_event, &tv) != 0)
		{
			printf("event timer fail\n");
		}
	}

	int ret = event_base_dispatch(main_event);
	printf("event_base_dispatch ret=%d\n", ret);

	event_base_free(main_event);

	return 0;
}


/**
 project base on libevent2.0
 this test is for two server, connect each other.
  **/
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
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

#define SERVER_HOST "0.0.0.0"
int g_flag = 0;
int g_port_list[] = {0, 7711, 7712};
struct bufferevent *g_bev = NULL;

void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data);
void read_cb(struct bufferevent *bev, void *user_data);
void event_cb(struct bufferevent *bev, short what, void *user_data);
void timer_cb(int fd, short event, void *user_data);

struct evconnlistener * create_listener(struct event_base *main_event);
struct bufferevent * create_connect_event(struct event_base *main_event);
struct event * create_timer_event(struct event_base *main_event);


void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)
{
	// handle new client connect event
	
	printf("listener_cb:fd=%d\n", fd);
	struct event_base *main_event = (struct event_base *)user_data;
	
	// 1. new a bufferevent
	// struct bufferevent *bufferevent_socket_new(struct event_base *base, evutil_socket_t fd, int options);
	struct bufferevent *bev = bufferevent_socket_new(main_event, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);

	// 2. set callback
	// void bufferevent_setcb(struct bufferevent *bufev, bufferevent_data_cb readcb, bufferevent_data_cb writecb, bufferevent_event_cb eventcb, void *cbarg);
	bufferevent_setcb(bev, read_cb, NULL, event_cb, user_data);

	// 3. add event into poll
	// int bufferevent_enable(struct bufferevent *bufev, short event);
	// NOTE: no need EV_PERSIST?
	// bufferevent_enable(bev, EV_READ | EV_WRITE | EV_PERSIST);
	// bufferevent_enable(bev, EV_READ | EV_WRITE);
	bufferevent_enable(bev, EV_READ);

}

void read_cb(struct bufferevent *bev, void *user_data)
{
	// handle read event
	
	// struct event_base *main_event = (struct event_base *)user_data;
	evutil_socket_t fd = bufferevent_getfd(bev);
	printf("read_cb:fd=%d\n", fd);

	// 1. get input evbuffer and length
	struct evbuffer *input = bufferevent_get_input(bev);
	const size_t input_len = evbuffer_get_length(input);
	printf("input_len=%lu\n", input_len);

	// local buffer must get all data from input because read_cb will not active again even if still has data in input, until receive client data next time
	char *in_buffer = (char *)calloc(1, input_len+1); 

	// 2. copy to local buffer
	int n = evbuffer_copyout(input, in_buffer, input_len);
	printf("n=%d in_buffer=%s\n", n, in_buffer);

	// 3. remove header data in evbuffer
	evbuffer_drain(input, n);

	free(in_buffer);
}

void event_cb(struct bufferevent *bev, short what, void *user_data)
{
	// handle other event

	// struct event_base *main_event = (struct event_base *)user_data;
	evutil_socket_t fd = bufferevent_getfd(bev);

	if (what & BEV_EVENT_CONNECTED)
	{
		printf("event_cb:event connected %d\n", fd);
	}
	else if (what & BEV_EVENT_EOF)
	{
		printf("event_cb:event eof fd=%d\n", fd);
		bufferevent_free(bev);
		g_bev = NULL;
	}
	else if (what & BEV_EVENT_ERROR)
	{
		printf("event_cb:event error fd=%d errno=%d\n", fd, errno);
		bufferevent_free(bev);
		g_bev = NULL;
	}

}

void timer_cb(int fd, short event, void *user_data)
{
	struct event_base *main_event = (struct event_base *)user_data;
	printf("timer_cb: fd=%d event=%d\n", fd, event);

	// reconnect logic
	if (g_bev == NULL)
	{
		g_bev = create_connect_event(main_event);
		if (g_bev == NULL)
		{
			printf("timer_cb: create_connect_event fail\n");
		}
		return;
	}

	// send msg logic
	struct evbuffer *output = bufferevent_get_output(g_bev);
	char out_buffer[100];
	sprintf(out_buffer, "hello %lu", time(NULL));
	evbuffer_add(output, out_buffer, strlen(out_buffer));
}

struct evconnlistener * create_listener(struct event_base *main_event)
{
	// 1. init a sin
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = PF_INET;
	int ip_num = inet_addr(SERVER_HOST);
	sin.sin_addr.s_addr = ip_num;
	sin.sin_port = htons(g_port_list[g_flag]);

	// 2. new and bind a listener
	// struct evconnlistener *evconnlistener_new_bind(struct event_base *base, evconnlistener_cb cb, void *ptr, unsigned flags, int backlog, const struct sockaddr *sa, int socklen);
	struct evconnlistener *listener = evconnlistener_new_bind(main_event, listener_cb, (void *)main_event, LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr *)&sin, sizeof(sin));
	if (listener == NULL)
	{
		return NULL;
	}

	// 3. set listen fd non-blocking
	evutil_socket_t listen_fd = evconnlistener_get_fd(listener);
	evutil_make_socket_nonblocking(listen_fd);

	return listener;
}

struct bufferevent * create_connect_event(struct event_base *main_event)
{
	// 1. init a sin
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = PF_INET;
	int ip_num = inet_addr(SERVER_HOST);
	sin.sin_addr.s_addr = ip_num;
	sin.sin_port = htons(g_port_list[3-g_flag]);

	// 2. init a socket
	int client_fd = socket(PF_INET, SOCK_STREAM, 0);
	evutil_make_socket_nonblocking(client_fd);

	// 3. create a socket bufferevent
	// struct bufferevent *bufferevent_socket_new(struct event_base *base, evutil_socket_t fd, int options);
	struct bufferevent *bev = bufferevent_socket_new(main_event, client_fd, BEV_OPT_CLOSE_ON_FREE);

	// 4. bufferevent set callback
	// void bufferevent_setcb(struct bufferevent *bufev, bufferevent_data_cb readcb, bufferevent_data_cb writecb, bufferevent_event_cb eventcb, void *cbarg);
	bufferevent_setcb(bev, read_cb, NULL, event_cb, main_event);

	// 5. add event into poll
	// int bufferevent_enable(struct bufferevent *bufev, short event);
	// NOTE: no need EV_PERSIST?
	// bufferevent_enable(bev, EV_READ | EV_WRITE | EV_PERSIST);
	bufferevent_enable(bev, EV_READ | EV_WRITE);

	// 6. connect
	int ret = bufferevent_socket_connect(bev, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
	{
		printf("bufferevent connect fail\n");
		bufferevent_free(bev);
		return NULL;
	}

	return bev;
}

struct event * create_timer_event(struct event_base *main_event)
{
	struct event *timer_event = event_new(main_event, -1, EV_PERSIST, timer_cb, (void *)main_event);
	
	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;

	event_add(timer_event, &tv);

	return timer_event;
}

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	if (argc < 2)
	{
		printf("g_flag no exists\n");
		return 0;
	}

	g_flag = atoi(argv[1]);
	printf("g_flag=%d\n", g_flag);
	if (g_flag != 1 && g_flag != 2)
	{
		printf("g_flag not 1 or 2\n");
		return 0;
	}

	// 1. main base event init
	// struct event_base *event_base_new(void);
	struct event_base *main_event = event_base_new();
	if (main_event == NULL)
	{
		printf("event_base_new fail\n");
		return 0;
	}

	// 2. create socket listener
	struct evconnlistener *listener = create_listener(main_event);
	if (listener == NULL)
	{
		printf("create_listener fail\n");
		return 0;
	}

	// 3. create client connect event
	g_bev = create_connect_event(main_event);
	if (g_bev == NULL)
	{
		printf("create_connect_event fail\n");
		return 0;
	}

	// 4. add a timer, for reconnect and send msg
	struct event *timer_event = create_timer_event(main_event);
	if (timer_event == NULL)
	{
		printf("create_timer_event fail\n");
		return 0;
	}

	// 5. main loop
	// int event_base_dispatch(struct event_base *);
	event_base_dispatch(main_event);

	// 6. clean up
	// void event_base_free(struct event_base *);
	if (g_bev != NULL)
	{
		bufferevent_free(g_bev);
	}
	event_base_free(main_event);

	return 0;
}


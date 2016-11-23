/**
 project base on libevent2.0
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
#define SERVER_PORT 7711

// listener callback
// typedef void (*evconnlistener_cb)(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data);

// bufferevent data callback
// typedef void (*bufferevent_data_cb)(struct bufferevent *bev, void *ctx);
void read_cb(struct bufferevent *bev, void *user_data);

// bufferevent event callback
// typedef void (*bufferevent_event_cb)(struct bufferevent *bev, short what, void *ctx);
void event_cb(struct bufferevent *bev, short what, void *user_data);


void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)
{
	// handle new client connect event
	
	printf("listener_cb: fd=%d\n", fd);
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
	printf("read_cb: fd=%d\n", fd);

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

	// 4. get output evbuffer and add to send msg to client
	struct evbuffer *output = bufferevent_get_output(bev);
	char out_buffer[100];
	sprintf(out_buffer, "hello %lu", time(NULL));
	evbuffer_add(output, out_buffer, strlen(out_buffer));

	free(in_buffer);
}

void event_cb(struct bufferevent *bev, short what, void *user_data)
{
	// handle other event
	printf("event_cb: what=%d\n", what);

	// struct event_base *main_event = (struct event_base *)user_data;
	evutil_socket_t fd = bufferevent_getfd(bev);

	if (what & BEV_EVENT_CONNECTED)
	{
		printf("event_cb: event connected %d\n", fd);
	}
	else if (what & BEV_EVENT_EOF)
	{
		printf("event_cb: event eof fd=%d\n", fd);
		bufferevent_free(bev);
	}
	else if (what & BEV_EVENT_ERROR)
	{
		printf("event_cb: event error fd=%d errno=%d\n", fd, errno);
		bufferevent_free(bev);
	}

}

struct evconnlistener * create_listener(struct event_base *main_event)
{
	// 1. init a sin
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = PF_INET;
	sin.sin_port = htons(SERVER_PORT);
	int ip_num = inet_addr(SERVER_HOST);
	sin.sin_addr.s_addr = ip_num;

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

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

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

	// 3. main loop
	// int event_base_dispatch(struct event_base *);
	event_base_dispatch(main_event);

	// 4. clean up
	// void event_base_free(struct event_base *);
	event_base_free(main_event);

	return 0;
}


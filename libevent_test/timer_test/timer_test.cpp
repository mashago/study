/**
 project base on libevent2.0
  **/
extern "C"
{
#include <stdio.h>
#include <strings.h>
#include <event2/event.h>
}

// typedef void (*event_callback_fn)(evutil_socket_t, short, void *);
void timer_cb(evutil_socket_t fd, short event, void *arg)
{
	int *pv = (int *)arg;
	printf("fd=%d event=%d v=%d\n", fd, event, *pv);
	(*pv)++;
}

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

	int v = 0;

	// 1. main base event init
	// struct event_base *event_base_new(void);
	struct event_base *main_event = event_base_new();
	if (main_event == NULL)
	{
		printf("event_base_new fail\n");
		return 0;
	}

	// 2. new a event
	// struct event *event_new(struct event_base *, evutil_socket_t, short, event_callback_fn, void *);
	// EV_PERSIST set event persistent, else event will be delete after wake
	// not necessary to pass a EV_TIMEOUT flag if the event is only a timeout event
	// NOTE: must call event_free to free the newly event in the end
	struct event *timer_event = event_new(main_event, -1, EV_PERSIST, timer_cb, (void *)&v);
	
	// 3. event add, set timer after 3sec
	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;
	// int event_add(struct event *ev, const struct timeval *timeout);
	int ret = event_add(timer_event, &tv);
	if (ret != 0)
	{
		printf("event_add fail\n");
	}

	// 4. main loop
	// int event_base_dispatch(struct event_base *);
	event_base_dispatch(main_event);

	// 5. free event
	// void event_free(struct event *);
	event_free(timer_event);

	// 6.clean up
	// void event_base_free(struct event_base *);
	event_base_free(main_event);

	return 0;
}

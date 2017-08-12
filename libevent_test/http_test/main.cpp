/**
 project base on libevent2.0
  **/
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <io.h> 
#include <process.h>
#include <winsock2.h>
#else
#include <unistd.h>
#endif
#include <errno.h>

#include <evdns.h>
#include <evhttp.h>
}

void http_conn_close_callback(struct evhttp_connection *http_conn, void *user_data)
{
	printf("http_conn_close_callback\n");
}

void http_done_callback(struct evhttp_request *http_request, void *user_data)
{
	printf("http_done_callback\n");
}

int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	const char *url = "http://www.baidu.com";

	// 1. main base event init
	// 2. new evdns
	// 3. new evhttp


	struct event_base *main_event = event_base_new();
	if (main_event == NULL)
	{
		printf("event_base_new fail\n");
		return 0;
	}

	struct evhttp_uri *uri = evhttp_uri_parse(url);
	if (!uri)
	{
		printf("evhttp_uri_parse fail\n");
		return 0;
	}

	const char *host = evhttp_uri_get_host(uri);
	if (!host)
	{
		printf("evhttp_uri_get_host fail\n");
		return 0;
	}
	int port = evhttp_uri_get_port(uri);
	if (port == -1)
	{
		port = 80;
	}
	const char *path = evhttp_uri_get_path(uri);
	if (!path)
	{
		path = "/";
	}

	struct evdns_base *dns = evdns_base_new(main_event, 1);
	if (!dns)
	{
		printf("evdns_base_new fail\n");
		return 0;
	}

	struct evhttp_connection *http_conn = evhttp_connection_base_new(main_event, dns, host, port);
	// struct evhttp_connection *http_conn = evhttp_connection_base_new(main_event, NULL, host, port);
	if (!http_conn)
	{
		printf("evhttp_connection_base_new fail\n");
		return 0;
	}
	evhttp_connection_set_timeout(http_conn, 600);
	evhttp_connection_set_closecb(http_conn, http_conn_close_callback, NULL);

	struct evhttp_request *http_request = evhttp_request_new(http_done_callback, NULL);
	evhttp_make_request(http_conn, http_request, EVHTTP_REQ_GET, path);

	event_base_dispatch(main_event);

	event_base_free(main_event);

	return 0;
}


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

#include <event2/event.h>
#include <event2/dns.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
}
#include <list>
#include <map>
#include <string>

#define G_URL "http://www.baidu.com"

struct request_data
{
	std::string host;
	int port;
	std::string path;
};
std::list<request_data> g_request_list;

std::map<std::string, struct evhttp_connection *> g_http_conn_map; // "host:port" : conn

static std::string get_http_conn_key(const char *host, int port)
{
	std::string key(host);
	key.append(":");
	key.append(std::to_string(port));
	return key;
}

static struct evhttp_connection * create_connection(struct event_base *main_event, struct evdns_base *dns, const char *host, int port);
static void http_conn_close_callback(struct evhttp_connection *http_conn, void *user_data);
static void http_done_callback(struct evhttp_request *http_request, void *user_data);

static void add_timer_cb(evutil_socket_t fd, short event, void *user_data);
static void check_timer_cb(evutil_socket_t fd, short event, void *user_data);

void http_conn_close_callback(struct evhttp_connection *http_conn, void *user_data)
{
	printf("******* http_conn_close_callback *******\n");

	for (auto iter = g_http_conn_map.begin(); iter != g_http_conn_map.end(); ++iter)
	{
		if (iter->second == http_conn)
		{
			g_http_conn_map.erase(iter);
			break;
		}
	}
}

void http_done_callback(struct evhttp_request *http_request, void *user_data)
{
	printf("******* http_done_callback *******\n");

	// struct event_base *main_event = (struct event_base*)user_data;

	int response_code = evhttp_request_get_response_code(http_request);
	printf("response_code=%d\n", response_code);
	if (response_code == HTTP_OK)
	{
		// handle header
		struct evkeyvalq *input_headers = evhttp_request_get_input_headers(http_request);
		struct evkeyval *node = input_headers->tqh_first;
		while (node)
		{
			printf("key=[%s] value=[%s]\n", node->key, node->value);
			node = node->next.tqe_next;
		}
		printf("\n");

		// handle content
		struct evbuffer *input_buffer = http_request->input_buffer;
		size_t input_len = evbuffer_get_length(input_buffer);
		printf("input_len=%lu\n", input_len);

		char *content = (char *)calloc(1, input_len+1); 
		int n = evbuffer_copyout(input_buffer, content, input_len);
		// printf("n=%d content=%s\n", n, content);

		evbuffer_drain(input_buffer, n);

		free(content);
	}

	// no need to free response!!!

	// event_base_loopbreak(main_event);
}

struct evhttp_connection * create_connection(struct event_base *main_event, struct evdns_base *dns, const char *host, int port)
{
	std::string key = get_http_conn_key(host, port);
	auto iter = g_http_conn_map.find(key);
	if (iter != g_http_conn_map.end())
	{
		printf("create_connection: use old connection\n");
		return iter->second;
	}

	printf("create_connection: new connection\n");
	struct evhttp_connection *http_conn = evhttp_connection_base_new(main_event, dns, host, port);
	if (!http_conn)
	{
		printf("evhttp_connection_base_new fail\n");
		return 0;
	}
	evhttp_connection_set_timeout(http_conn, 600);
	evhttp_connection_set_closecb(http_conn, http_conn_close_callback, (void *)main_event);

	g_http_conn_map[key] = http_conn;

	return http_conn;
}

void add_timer_cb(evutil_socket_t fd, short event, void *user_data)
{
	struct evhttp_uri *uri = NULL;
	do
	{
		uri = evhttp_uri_parse(G_URL);
		if (!uri)
		{
			printf("evhttp_uri_parse fail\n");
			break;
		}

		const char *host = evhttp_uri_get_host(uri);
		if (!host)
		{
			printf("evhttp_uri_get_host fail\n");
			break;
		}
		
		int port = evhttp_uri_get_port(uri);
		if (port == -1)
		{
			port = 80;
		}

		const char *path = evhttp_uri_get_path(uri);
		if (!path || strlen(path) == 0)
		{
			path = "/";
		}
	
		struct request_data d;
		d.host = std::string(host);
		d.port = port;
		d.path = std::string(path);
		g_request_list.push_back(d);
		g_request_list.push_back(d);
		g_request_list.push_back(d);
		g_request_list.push_back(d);
		g_request_list.push_back(d);

	}
	while (false);

	if (uri)
	{
		evhttp_uri_free(uri);
	}
}

void check_timer_cb(evutil_socket_t fd, short event, void *user_data)
{
	struct event_base *main_event = (struct event_base*)user_data;

	if (g_request_list.empty())
	{
		return;
	}

	while (!g_request_list.empty())
	{
		struct request_data d = g_request_list.front();
		g_request_list.pop_front();

		struct evhttp_connection *http_conn = create_connection(main_event, NULL, d.host.c_str(), d.port);
		if (!http_conn)
		{
			return;
		}

		struct evhttp_request *new_request = evhttp_request_new(http_done_callback, (void *)main_event);
		evhttp_add_header(evhttp_request_get_output_headers(new_request), "Host", d.host.c_str());
		evhttp_make_request(http_conn, new_request, EVHTTP_REQ_GET, d.path.c_str());
	}
}


int main(int argc, char **argv)
{
	printf("hello %s\n", argv[0]);

#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	// 1. main base event init
	// 2. init uri, host, port, path
	// 3. new dns
	// 4. new connection
	// 5. new request


	// 1. main base event init
	struct event_base *main_event = event_base_new();
	if (main_event == NULL)
	{
		printf("event_base_new fail\n");
		return 0;
	}

	// 2. init uri, host, port, path
	struct evhttp_uri *uri = evhttp_uri_parse(G_URL);
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
	if (!path || strlen(path) == 0)
	{
		path = "/";
	}
	printf("host=%s port=%d path=%s\n", host, port, path);

	// 3. new dns
	struct evdns_base *dns = evdns_base_new(main_event, 1);
	if (!dns)
	{
		printf("evdns_base_new fail\n");
		return 0;
	}

	// 4. new connection
	struct evhttp_connection *http_conn = create_connection(main_event, NULL, host, port);
	if (!http_conn)
	{
		printf("create_connection fail\n");
		return 0;
	}

	// 5. new request
	struct evhttp_request *http_request = evhttp_request_new(http_done_callback, (void *)main_event);
	evhttp_add_header(evhttp_request_get_output_headers(http_request), "Host", host);
	evhttp_make_request(http_conn, http_request, EVHTTP_REQ_GET, path);

	// 6. free uri
	evhttp_uri_free(uri);

	///////////////////////////////////////

	// 7. add timer
	struct event *add_timer_event = event_new(main_event, -1, EV_PERSIST, add_timer_cb, (void *)main_event);
	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;
	int ret = event_add(add_timer_event, &tv);
	if (ret != 0)
	{
		printf("event_add fail\n");
	}

	// 8. check timer
	struct event *check_timer_event = event_new(main_event, -1, EV_PERSIST, check_timer_cb, (void *)main_event);
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	ret = event_add(check_timer_event, &tv);
	if (ret != 0)
	{
		printf("event_add fail\n");
	}

	printf("loop start\n");
	event_base_dispatch(main_event);

	event_base_free(main_event);

	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     //for getopt, fork
#include <string.h>     //for strcat
//for struct evkeyvalq
#include <sys/queue.h>
#include <event.h>
//for http
#include <evhttp.h>
#include <signal.h>

#include <string>

#define HTTPD_SIGNATURE   "http server"

int connect_timeout = 20;

void send_not_found(struct evhttp_request *req)
{
	//HTTP header
	evhttp_add_header(req->output_headers, "Server", HTTPD_SIGNATURE);
	evhttp_add_header(req->output_headers, "Content-Type", "text/plain; charset=UTF-8");
	evhttp_add_header(req->output_headers, "Connection", "close");

	struct evbuffer *buf;
	buf = evbuffer_new();
	evbuffer_add_printf(buf, "Not Found!\n");
	evhttp_send_reply(req, HTTP_NOTFOUND, "NOT FOUND", buf);

	evbuffer_free(buf);
}

void request_handler(struct evhttp_request *req, void *arg)
{
	send_not_found(req);
}

void request_root_handler(struct evhttp_request *req, void *arg)
{
	char output[2048] = "\0";
	char tmp[1024];

	// 0.get http connect and set timeout
	// 1.get method
	// 2.get uri
	// 3.decoded uri
	// 4.get path and query
	// 5.get param from query
	// 6.get post data
	// 7.do logic
	// 8.response to client
	// 9.clean malloc data
	
	sprintf(tmp, "into request_root_handler\n");
	strcat(output, tmp);

	// 0.
	evhttp_connection_set_timeout(req->evcon, connect_timeout);

	// 1.
	enum evhttp_cmd_type method = evhttp_request_get_command(req);
	sprintf(tmp, "method=%d\n", method);
	strcat(output, tmp);

	// 2.
	const char *uri;
	// uri = evhttp_request_get_uri(req);
	uri = evhttp_request_uri(req);
	// uri = req->uri; // same way
	sprintf(tmp, "uri=%s\n", uri);
	strcat(output, tmp);

	// 3.decoded uri
	char *decoded_uri;
	decoded_uri = evhttp_decode_uri(uri);
	sprintf(tmp, "decoded_uri=%s\n", decoded_uri);
	strcat(output, tmp);

	// 4.get path and query
	// usr evhttp_uri to parse uri
	struct evhttp_uri *puri = NULL;
	puri = evhttp_uri_parse(decoded_uri);
	if (puri == NULL)
	{
		// error
		free(decoded_uri);
		return;
	}

	const char *path = NULL;
	const char *query = NULL;
	path = evhttp_uri_get_path(puri);
	query = evhttp_uri_get_query(puri);
	sprintf(tmp, "path=%s\n", path);
	strcat(output, tmp);
	sprintf(tmp, "query=%s\n", query);
	strcat(output, tmp);

	// 5.
	struct evkeyvalq params;
	evhttp_parse_query_str(query, &params);
	sprintf(tmp, "q=%s\n", evhttp_find_header(&params, "q"));
	strcat(output, tmp);
	sprintf(tmp, "s=%s\n", evhttp_find_header(&params, "s"));
	strcat(output, tmp);

	// 6.
	char *post_data = (char *) EVBUFFER_DATA(req->input_buffer);
	sprintf(tmp, "post_data=%s\n", post_data);
	strcat(output, tmp);

	// 7. logic


	// 8. response to client
	//HTTP header
	evhttp_add_header(req->output_headers, "Server", HTTPD_SIGNATURE);
	evhttp_add_header(req->output_headers, "Content-Type", "text/plain; charset=UTF-8");
	evhttp_add_header(req->output_headers, "Connection", "close");

	struct evbuffer *buf;
	buf = evbuffer_new();
	evbuffer_add_printf(buf, "It works!\n%s\n", output);
	evhttp_send_reply(req, HTTP_OK, "OK", buf);

	// 9.
	evbuffer_free(buf);
	evhttp_uri_free(puri);
	free(decoded_uri); // must free it

}

//当向进程发出SIGTERM/SIGHUP/SIGINT/SIGQUIT的时候，终止event的事件侦听循环
void signal_handler(int sig)
{
	switch (sig) {
		case SIGTERM:
		case SIGHUP:
		case SIGQUIT:
		case SIGINT:
			event_loopbreak();  //终止侦听event_dispatch()的事件侦听循环，执行之后的代码
			break;
	}
}

int main(int argc, char *argv[])
{
	//自定义信号处理函数
	signal(SIGHUP, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);

	//默认参数
	const char *ip = "0.0.0.0";
	int port = 8080;
	int is_daemon = 0;
	connect_timeout = 10; //in seconds

	//获取参数
	int c;
	while ((c = getopt(argc, argv, "h:p:t:d")) != -1)
	{
		switch (c)
		{
		case 'h' :
			ip = optarg;
			break;
		case 'p' :
			port = atoi(optarg);
			break;
		case 't' :
			connect_timeout = atoi(optarg);
			break;
		case 'd' :
			is_daemon = 1;
			break;
		}
	}

	printf("ip=[%s] port=[%d] connect_timeout=[%d] is_daemon=[%d]\n", ip, port, connect_timeout, is_daemon);

	if (is_daemon)
	{
		pid_t pid;
		pid = fork();
		if (pid < 0)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		if (pid > 0)
		{
			exit(EXIT_SUCCESS);
		}
	}


	// setup http server by libevent
	// 1.init event
	// 2.event start http
	// 3.set request handler
	// 4.dispatch
	// 5.free

	// 1.
	event_init();

	// 2.
	struct evhttp *httpd;
	httpd = evhttp_start(ip, port);

	// 3.
	evhttp_set_gencb(httpd, request_handler, NULL);
	// for specific url "/"
	evhttp_set_cb(httpd, "/", request_root_handler, NULL);

	// 4.
	event_dispatch();

	// 5.
	evhttp_free(httpd);

	return 0;
}

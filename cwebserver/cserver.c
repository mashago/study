
#include <stdio.h>
// #include <sys/socket.h>
#include <netinet/in.h> // for sockaddr_in
#include <netinet/tcp.h> // for TCP_NODELAY
#include <arpa/inet.h> // for inet_addr()
#include <string.h> // memset
#include <unistd.h> // for sleep
#include <fcntl.h>
#include <errno.h>
#include <sys/un.h>

#include "fdwatch.h"
#include "base64.h"
#include "sha1.h"
#include "intLib.h"


#define UNIX_DOMAIN	"/tmp/dbio.sock"

#define SERVER_HOST	"127.0.0.1"
#define SERVER_PORT	7710

#define MAX_CONNECT			100
#define BUFFER_SIZE			1000

#define WATCH_TIMEOUT		60000

#define ACCEPT_FAIL			-1
#define ACCEPT_NO_MORE		-2

#define STATE_FREE			0
#define STATE_READING		1
#define STATE_SENDING		2
#define STATE_LISTEN		20

#define CONN_NONE		0
#define CONN_NORMAL		1
#define CONN_WEBSOCKET	2

typedef struct {
	int fd;
	int state;
	int websocket_flag;
	int next_free;

	/*
	|       out_len     |
	|___________________________| out_buffer
	|out_offset|
	*/
	char out_buffer[BUFFER_SIZE];
	int out_offset;
	int out_len;



} connect_t;

#define DB_TRANS_MAX	100
#define DB_BUFFER_MAX	5000
typedef struct {
	int c_fd; // for dbio
	int accept_fd; // for cserver
	char db_buffer[DB_TRANS_MAX][DB_BUFFER_MAX];
} dbio_t;

connect_t g_connect_list[MAX_CONNECT];
int g_free_connect_id = 0;

////////////////////////// CONNECT LOGIC START [


int init_connect_list()
{
	int ret;

	// printf("sizeof(g_connect_list)=%zu\n", sizeof(g_connect_list));
	memset(&g_connect_list, 0, sizeof(g_connect_list));
	for (int i=0; i<MAX_CONNECT; i++) {
		connect_t * conn = g_connect_list + i;
		conn->state = STATE_FREE;
		conn->next_free = i+1;
	}
	g_connect_list[MAX_CONNECT-1].next_free = -1;
	g_free_connect_id = 0;

	ret = 0;
	return ret;
}

connect_t * get_conn(int cid)
{
	if (cid < 0 || cid >= MAX_CONNECT) {
		printf("get_conn:cid_out_bound %d", cid);
		return NULL;
	}

	return g_connect_list + cid;
}

connect_t * new_conn(int fd)
{
	connect_t * conn = NULL;
	conn = get_conn(g_free_connect_id);
	if (conn == NULL) {
		printf("new_conn:no_more_free_connect");
		return NULL;
	}
	if (conn->state != STATE_FREE) {
		printf("new_conn:connect_not_free");
		return NULL;
	}
	conn->fd = fd;
	conn->state = STATE_READING;
	g_free_connect_id = conn->next_free;
	conn->next_free = -1;
	conn->out_offset = 0;
	conn->out_len = 0;

	return conn;
}

int free_conn(connect_t * conn)
{
	memset(conn, 0, sizeof(connect_t));
	conn->state = STATE_FREE;
	conn->next_free = g_free_connect_id;
	g_free_connect_id = conn - g_connect_list;

	return 0;
}

int clean_conn(connect_t * conn)
{
	fdwatch_del_fd(conn->fd);
	close(conn->fd);
	free_conn(conn);
	return 0;
}

////////////////////////// CONNECT LOGIC END ]

////////////////////////// SOCKET LOGIC START [

int init_server_socket()
{
	int ret;
	int fd;

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	/*
	struct sockaddr_in {
		short int sin_family; 	// address family
		unsigned short int sin_port;
		struct in_addr sin_addr;	// internet address
		unsigned char size_zero[8];
	};
	struct in_addr {
		unsigned long s_addr; 
	};
	*/

	// init sockaddr_in
	sin.sin_family = AF_INET;
	int ip = inet_addr(SERVER_HOST);
	sin.sin_addr.s_addr = ip;
	sin.sin_port = htons(SERVER_PORT);

	// init socket
	// int fd = socket(sin.sin_family, SOCK_STREAM, 0);
	fd = socket(PF_INET, SOCK_STREAM, 0);
	printf("init_server_socket:listen_fd=%d\n", fd);
	if (fd < 0) {
		return -6;
	}
	int flag = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&flag, sizeof(flag));

	// bind 
	ret = bind(fd, (struct sockaddr*)&sin, sizeof(sin)); // change to sockaddr
	if (ret < 0) {
		printf("init_server_socket:bind_error %d\n", ret);
		return ret;
	}


	// update fcntl add flag no delay, accept will no delay
	flag = fcntl(fd, F_GETFL, 0);
	if (flag < 0) {
		printf("init_server_socket:fchtl_get_flag_fail");
		return -16;
	}
	ret = fcntl(fd, F_SETFL, flag | O_NDELAY);
	if (ret < 0) {
		printf("init_server_socket:fchtl_set_flag_fail\n");
		return -26;
	}

	// listen
	int backlog = 1024;
	ret = listen(fd, backlog);
	if (ret != 0) {
		printf("init_server_socket:listen_error %d\n", ret);
		ret = -7;
		return ret;
	}

	return fd;
}

int accept_client(int fd)
{
	int ret;
	int c_fd;

	// accept client
	int flag = 0;
	struct sockaddr_in c_sin;
	socklen_t len = sizeof(c_sin);
	c_fd = accept(fd, (struct sockaddr*)&c_sin, &len);
	if (c_fd < 0) {
		// when server socket set O_NDELAY, accept fd may < 0
		if (errno==EWOULDBLOCK || errno==EAGAIN) {
			errno = 0;
			return ACCEPT_NO_MORE;
		}
		printf("accept_client:accept_fail %d\n", errno);
		return ACCEPT_FAIL;
	}

	flag = 1;
	ret = setsockopt(c_fd, IPPROTO_TCP, TCP_NODELAY, (const void*)&flag, sizeof(flag));
#ifdef __linux__
	// keepalive
	flag = 1;
	ret = setsockopt(c_fd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(int));
	flag = 8; // max is 8 ?
	ret = setsockopt(c_fd, SOL_TCP, TCP_KEEPCNT, &flag, sizeof(int));
	flag = 3;  // after 3 seconds of idle, start keepalive
	ret = setsockopt(c_fd, SOL_TCP, TCP_KEEPIDLE, &flag, sizeof(int));
	flag = 3;  // interval is 3 seconds
	ret = setsockopt(c_fd, SOL_TCP, TCP_KEEPINTVL, &flag, sizeof(int));
#endif

	return c_fd;
}

int handle_new_client(int fd)
{
	while (1){

	int c_fd = accept_client(fd);
	switch (c_fd) {
		case ACCEPT_FAIL:
			return ACCEPT_FAIL;
		case ACCEPT_NO_MORE:
			return 0;
		default:
			break;
	}
	printf("handle_new_client:client_fd=%d\n", c_fd);

	connect_t * conn = new_conn(c_fd);
	if (conn == NULL) {
		printf("handle_new_client:new_conn_fail %d\n", c_fd);
		close(c_fd);
		return -6;
	}
	conn->state = STATE_READING;
	fdwatch_add_fd(conn->fd, conn, FDW_READ);

	}
	return -6;
}

////////////////////////// SOCKET LOGIC END ]


////////////////////////// CLIENT LOGIC START [

int net_write(connect_t * conn, const char * str)
{
	int len = strlen(str);
	printf("net_write:len=%d out_len=%d out_offset=%d\n", len, conn->out_len, conn->out_offset);

	// 2 is \n\0
	if (conn->out_len + len + 2 > BUFFER_SIZE) {
		printf("ERROR net_write:buffer_out_bound %d %d\n", conn->out_len, len);
		return 0;
	}

	strcpy(conn->out_buffer + conn->out_len, str);
	conn->out_len += len + 1;
	conn->out_buffer[conn->out_len] = '\n';
	conn->out_len += 1;
	
	conn->state = STATE_SENDING;
	fdwatch_del_fd(conn->fd);
	fdwatch_add_fd(conn->fd, conn, FDW_WRITE);

	return 0;
}

int process_command(connect_t *conn, char *buffer)
{
	int ret;
	char * tok;
	char * context;
	const char * seq = "\r\n"; // [13] [10]
	for (tok=strtok_r(buffer, seq, &context); tok!=NULL; tok=strtok_r(NULL, seq, &context)) {
		if (tok[0] == '\0' || tok[0] == '\n') {
			continue;
		}
		printf("do_command:conn_id=%ld tok=%s\n", conn-g_connect_list, tok);
		if (*tok-'\0' == 4) { // EOT
			printf("do_command:EOT conn_id=%ld\n", conn-g_connect_list);
			clean_conn(conn);
			return 0;
		}
		if (strcmp(tok, "q") == 0) {
			clean_conn(conn);
			return 0;
		}

		ret = net_write(conn, tok);
	}
	ret = 0;
	return ret;
}


#define RESPONSE_HEADER_LEN_MAX 1024
int shake_hand(connect_t *conn, const char *serverKey)
{
	char header [RESPONSE_HEADER_LEN_MAX];

	if(conn->fd <= 0) {
		return -3;
	}

	if(serverKey == NULL)
	{
		return -13;
	}

	memset(header, '\0', RESPONSE_HEADER_LEN_MAX);

	sprintf(header, "HTTP/1.1 101 Switching Protocols\r\n");
	sprintf(header, "%sUpgrade: websocket\r\n", header);
	sprintf(header, "%sConnection: Upgrade\r\n", header);
	sprintf(header, "%sSec-WebSocket-Accept: %s\r\n\r\n", header,
	serverKey);

	printf("Response Header:%s\n",header);

	// write(connfd,header,strlen(header));
	net_write(conn, header);
	return 0;
}

#define WEB_SOCKET_KEY_LEN_MAX 256
int websocket_getkey(char *key, const char * buf)
{
	char *begin;
	char *flag="Sec-WebSocket-Key: ";
	int i=0, len=0;

	begin=strstr(buf,flag);
	if(!begin) {
		return -3;
	}
	begin+=strlen(flag);

	len=strlen(buf);
	for(i=0;i<len;i++)
	{
		if(begin[i]=='\r' || begin[i]=='\n') {
			break;
		}
		key[i]=begin[i];
	}
	key[i+1] = '\0';

	return 0;
}

#define GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
int websocket_computekey(char *server_key, const char *client_key)
{
	char hashkey[200];
	char sha[200];
	char sha_data[400];
	int len;
	int ret;
	if (client_key == NULL) {
		return -3;
	}

	if (server_key == NULL) {
		return -13;
	}

	sprintf(hashkey, "%s%s", client_key, GUID);

	ret = sha1_hash(sha, hashkey);

			if (ret != 0) {
				printf("db_read:sha1_hash_fail\n");
			} else {
				printf("db_read:sha1_buffer=%s\n", sha1_buffer);
				int len = strlen(sha1_buffer);
				char sha1_data[len+1];
				// menset(sha1_data, 0, sizeof(sha1_data));

				for(int i=0;i<len;i+=2) {       
					sha1_data[i/2]=htoi(sha1_buffer,i,2);
				} 
				char *server_key_malloc = base64_encode(sha1_data, strlen(sha1_data));
				printf("db_read:server_key=%s\n", server_key_malloc);
				char server_key[500];
				sprintf(server_key, "%s", server_key_malloc);
				free(server_key_malloc);

				printf("db_read:server_key=%s\n", server_key);
				shake_hand(conn, server_key);

			}
}

			
int do_read(connect_t *conn)
{
	int ret;
	char buffer[1000];
	memset(buffer, 0, sizeof(buffer));
	int size;
	size = read(conn->fd, buffer, 990);
	// core logic: when client disconnect, read size == 0 at last
	if (size == 0) {
		printf("do_read:clean_conn\n");
		clean_conn(conn);
		return 0;
	}
	if (size < 0) {
		printf("do_read:read_size<0\n");
		if (errno == EINTR || errno == EAGAIN 
		|| errno == EWOULDBLOCK) {
			errno = 0;
			return 0;
		}
		clean_conn(conn);
		return -6;
	}
	buffer[size] = '\0'; 

	printf("do_read:%s\n", buffer);

	// handle websocket head
	if (conn->websocket_flag == CONN_NONE) {
		char client_key[200];
		char server_key[200];
		ret = websocket_getkey(buffer, key_buffer);
		if (ret != 0) {
			printf("db_read:key_not_found\n");
			conn->websocket_flag = CONN_NORMAL;
		} else {
			// handle websocket header

		}
	}



	/*
	printf("do_read:size=%d\n", size);
	for (int i=0; i<size; i++) {
		printf("%x ", (int)buffer[i]);
	}
	printf("\n");
	*/

	// ret = process_command(conn, buffer);
	ret = 0;
	return ret;
}

int do_send(connect_t *conn)
{
	int size;

	size = write(conn->fd, conn->out_buffer+conn->out_offset
	, conn->out_len-conn->out_offset);
	printf("do_send:size=%d out_len=%d out_offset=%d\n", size, conn->out_len, conn->out_offset);

	if (size < 0) {
		free_conn(conn);
		return -5;	
	}

	conn->out_offset += size;

	if (conn->out_offset >= conn->out_len) {
		// all out_buffer finish send, refresh out_len, out_offset
		conn->out_offset = 0;
		conn->out_len = 0;
		conn->out_buffer[0] = '\0';
	}

	conn->state = STATE_READING;
	fdwatch_del_fd(conn->fd);
	fdwatch_add_fd(conn->fd, conn, FDW_READ);

	return 0;
}

int handle_client(connect_t *conn)
{
	int ret;

	if (conn == NULL) {
		// after read() size=0 and clean_conn()
		// printf("main:fdwatch_get_next_client_data_null\n");
		return 0;
	}

	if (conn->state == STATE_LISTEN) {
		printf("handle_client:state_listen\n");
		return 0;
	}

	// check client fd is active, if not, bug, clean that connect
	ret = fdwatch_check_fd(conn->fd);
	if (ret == 0) {
		printf("main:fdwatch_check_fd %d %d\n", conn->fd, ret);
		clean_conn(conn);
		return -6;
	}
	
	if (conn->state == STATE_READING) {
		do_read(conn);
		return 0;
	}

	if (conn->state == STATE_SENDING) {
		do_send(conn);
		return 0;
	}

	return 0;
}
////////////////////////// CLIENT LOGIC END ]

////////////////////////// DB LOGIC START [

int init_db_server_socket()
{
	int ret;
	int fd;

	struct sockaddr_un sun;
	/*
	// unix sock
	struct sockaddr_un {
		sa_family_t sun_family;
		char sun_path[UNIX_PATH_MAX];
	};
	*/
	memset(&sun, 0, sizeof(sun));
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path, UNIX_DOMAIN);
	unlink(UNIX_DOMAIN);

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	printf("init_db_server_socket:fd=%d\n", fd);
	if (fd < 0) {
		return -6;
	}

	ret = bind(fd, (struct sockaddr*)&sun, sizeof(sun));
	if (ret < 0) {
		return -16;
	}

	ret = listen(fd, 1);
	if (ret < 0) {
		return -26;
	}

	return fd;
}

int init_db_client_socket()
{
	int ret;
	int fd;
	struct sockaddr_un sun;
	memset(&sun, 0, sizeof(sun));
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path, UNIX_DOMAIN);
	fd = socket(PF_UNIX, SOCK_STREAM, 0);
	printf("init_db_client_socket:fd=%d\n", fd);
	if (fd < 0) {
		return -6;
	}

	ret = connect(fd, (struct sockaddr*)&sun, sizeof(sun));
	if (ret < 0) {
		return -16;
	}

	return fd;
}

int init_db_conn()
{
	int ret;
	int main_fd;
	int c_fd;
	int accept_fd;
	struct sockaddr_un sun;
	socklen_t len;

	len = sizeof(struct sockaddr_un);

	main_fd = init_db_server_socket();

	c_fd = init_db_client_socket();

	accept_fd = accept(main_fd, (struct sockaddr*)&sun, &len);
	printf("init_db_pthread:main_fd=%d c_fd=%d accept_fd=%d\n"
	, main_fd, c_fd, accept_fd);

	const char *out_buffer = "hello";
	ret = write(accept_fd, out_buffer, strlen(out_buffer));
	// ret = write(c_fd, out_buffer, strlen(out_buffer));
	printf("init_db_conn:ret=%d\n", ret);

	char buffer[100];
	memset(buffer, 0, sizeof(buffer));
	ret = read(c_fd, buffer, 100);
	// ret = read(accept_fd, buffer, 100);
	printf("init_db_conn:buffer=%s ret=%d\n", buffer, ret);

	ret= -10;
	return -10;

}
////////////////////////// DB LOGIC END ]


int main(int argc, char * argv[])
{

	printf("-------- cserver START --------\n");

	int ret;
	int listen_fd;

	ret = init_connect_list();
	if (ret < 0) {
		printf("main:init_connect_list_fail ret=%d\n", ret);
		return -6;
	}

	// init fdwatch
	ret = fdwatch_get_nfiles();
	if (ret <=0 ) {
		printf("main:fdwatch_get_nfile_fail ret=%d\n", ret);
		return -7;
	}
	// printf("fdwatch_get_nfile:ret=%d\n", ret);

	/*
	ret = init_db_conn();
	if (ret < 0) {
		printf("main:init_db_conn_fail ret=%d\n", ret);
		return -17;
	}
	*/
		

	// init server socket
	listen_fd = init_server_socket();
	if (listen_fd <= 0) {
		printf("main:init_server_socket_fail ret=%d\n", listen_fd);
		return -10;
	}
	connect_t conn_listen;
	memset(&conn_listen, 0, sizeof(conn_listen));
	conn_listen.state = STATE_LISTEN;
	fdwatch_add_fd(listen_fd, (void*)&conn_listen, FDW_READ);


	// main logic
	int running = 1;
	int num_ready = 0;
	connect_t *conn;
	while (running) {
		num_ready = fdwatch(WATCH_TIMEOUT);	

		printf("fdwatch:num_ready=%d\n", num_ready);

		// accept and handle new client
		if (0 != fdwatch_check_fd(listen_fd)) {
			if (0 == (ret=handle_new_client(listen_fd))) {
				continue;
			}
			printf("main:handle_new_client_fail %d\n", ret);
		}

		// here, client fd is active, get client data and handle it
		while ((conn=(connect_t*)fdwatch_get_next_client_data())
		!= (connect_t*)-1) {

			ret = handle_client(conn);

		}

	}

	ret = 0;
	return ret;
}

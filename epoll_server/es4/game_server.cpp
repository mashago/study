
#if __APPLE__
// @see http://www.opensource.apple.com/source/mDNSResponder/mDNSResponder-258.18/mDNSPosix/PosixDaemon.c
#define daemon avoid_warning_thankyou
#endif

extern "C"
{
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <sys/mman.h>
#include <sys/types.h>

#if __APPLE__
#undef daemon
extern int daemon(int, int);
#endif

#include "socket_poll.h"
}

#include <string>
#include <fstream>
#include <map>
#include <list>

#include "common.h"
#include "log_manager.h"
#include "config_reader.h"
#include "command.h"
#include "game_server.h"

#define MAX_MSG_BODY_SIZE 10240
#define WAIT_TIMEOUT 10000 

bool is_running = true; // if false, server will shutdown
std::string SERVER_IP = "0.0.0.0";
int SERVER_PORT = 7712;
int MAX_CONNECT = 10;

int g_pfd = -1;
connect_t g_listen_conn;

int g_free_connect = 0;  // the free connect id
connect_t *g_conn_list;
std::map<unsigned int, connect_t *> g_uin_map; // client uin to gate connect 
std::map<unsigned int, connect_t *> g_server_map; // server type to connect
std::list<connect_t *> g_gate_list; // gate server list

///// CONNECTION UTIL START [

void init_connection_list(int max_connect)
{
	// 1.init all connect
	// 2.set last connect next_free is -1
	// 3.set first free connect id is 0
	
	int in_buffer_size = 512000;
	g_conn_list = new connect_t[max_connect];
	for (int i = 0; i < max_connect; i++)
	{
		g_conn_list[i].init(in_buffer_size);
		g_conn_list[i].next_free = i + 1;
	}

	// set last connect next_free is -1
	g_conn_list[max_connect-1].next_free = -1;
	g_free_connect = 0;
}

unsigned int create_uid()
{
	static unsigned int uid = 10;
	uid++;
	return uid;
}

connect_t * new_conn(int fd)
{
	// 1.get free connect id
	// 2.set connect data
	// 3.set free connect id is next_free
	
	if (g_free_connect == -1)
	{
		return NULL;
	}
	connect_t * conn = &g_conn_list[g_free_connect];
	conn->fd = fd;
	conn->uid = create_uid();
	g_free_connect = conn->next_free;
	return conn;
}

int free_conn(connect_t * conn)
{
	// 1.close fd
	// 2.init connect
	// 3.set free connect
	
	if (conn == NULL)
	{
		return 0;
	}
	
	close(conn->fd);
	conn->init();
	conn->next_free = g_free_connect;
	g_free_connect = conn - g_conn_list;
	return 0;
}

connect_t * get_conn_by_uin(unsigned int uin)
{
	auto iter = g_uin_map.find(uin);
	if (iter == g_uin_map.end())
	{
		return NULL;
	}
	return iter->second;
}

///// CONNECTION UTIL END   ]

///// NETWORK START [

int init_listen_fd(std::string ip, int port)
{
	int ret;

	// init listen_fd
	// 1.create normal socket
	// 2.set reuse, set non-block
	// 3.bind, listen
	
	int listen_fd = -1;

	// 1. create socket
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	int ip_num = inet_addr(ip.c_str());
	sin.sin_addr.s_addr = ip_num;

	listen_fd = socket(sin.sin_family, SOCK_STREAM, 0);
	if (listen_fd < 0)
	{
		printf("init_listen_fd:socket fail errno=%d\n", errno);
		return listen_fd;
	}

	// 2.set reuse, non-block
	int flag;
	flag = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
	sp_nonblocking(listen_fd);

	// 3.bind, listen
	ret = bind(listen_fd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
	{
		printf("init_listen_fd:bind fail errno=%d\n", errno);
		return ret;
	}

	// int backlog = 2048;
	// int backlog = 5120;
	int backlog = MAX_CONNECT;
	ret = listen(listen_fd, backlog);
	if (ret < 0)
	{
		printf("init_listen_fd:listen fail errno=%d\n", errno);
		return ret;
	}

	g_listen_conn.init(1);
	g_listen_conn.fd = listen_fd;
	g_listen_conn.uid = 0; // not important

	return listen_fd;
}

// accept until EAGAIN
int do_accept(int listen_fd, int pfd)
{
	int max_accept_count = 10;
	int ret = 0;

	while (max_accept_count--)
	{
		// accept client
		// 1.accept
		// 2.set nonblock, tcp nodealy
		// 3.add into poll, event is EPOLLIN | EPOLLET
		struct sockaddr_in c_sin;
		bzero(&c_sin, sizeof(c_sin));
		socklen_t sin_size = sizeof(c_sin);
		int client_fd = accept(listen_fd, (struct sockaddr*)&c_sin, &sin_size);	
		if (client_fd < 0)
		{
			if (errno == EAGAIN || EWOULDBLOCK)
			{
				// no more connection
				ret = 0;
				break;
			}
			else
			{
				// accept error, something go wrong
				LOG(LLA, "************* do_accept:accept error %d **************", errno);
				ret = -1;
				break;
			}
		}

		connect_t * conn = new_conn(client_fd);
		if (conn == NULL)
		{
			// cannot handle more connect
			LOG(LLA, "************* do_accept:can not handle more connect ************");
			close(client_fd);
			continue;
		}

		LOG(LLD, "do_accept:new connect uid=%d", conn->uid);

		sp_nonblocking(client_fd);

		// int flag = 1;
		// ret = setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
		
		sp_add(pfd, client_fd, (void *)conn);

	}
	return ret;
}

int do_read(connect_t *conn, int pfd)
{
	// 1.read fd
	// 2.if read size == 0, remove from poll, close fd
	// 3.handle msg

	// int ret = 0;
	if (conn == NULL)
	{
		LOG(LLA, "do_read:conn==NULL");
		return -1;
	}
	if (!conn->is_valid())
	{
		LOG(LLB, "do_read:conn invalid %ld", conn - g_conn_list);
		return -1;
	}

	int size = read(conn->fd, conn->in_end_pos , conn->in_buffer_size - (conn->in_end_pos - conn->in_buffer));
	if (size == 0)
	{
		// EOF, server disconnect
		LOG(LLA, "******** do_read:server shutdown conn->fd=%d conn->uid=%d\n", conn->fd, conn->uid);
		server_disconnect(conn);
		sp_del(pfd, conn->fd);
		free_conn(conn);
		return -1;
	}

	if (size < 0)
	{
		if (errno == EINTR)
		{
			return 0;
		}

		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// read no more, normal?
			LOG(LLD, "do_read:EAGAIN");
			return 0;
		}

		// something go wrong
		LOG(LLA, "*************** do_read:read error, errno=%d ***********", errno);
		sp_del(pfd, conn->fd);
		free_conn(conn);
		return -1;
	}
	conn->in_end_pos += size;

	return 0;
}

int do_write(connect_t *conn, int pfd)
{
	if (conn == NULL)
	{
		LOG(LLA, "do_write:conn==NULL");
		return -1;
	}
	
	if (!conn->is_valid())
	{
		LOG(LLB, "do_write:conn invalid %ld\n", conn - g_conn_list);
		return -1;
	}

	if (conn->out_buffer.size() == 0)
	{
		// no more to send, cancel listen write
		sp_write(pfd, conn->fd, (void *)conn, false);
		return 0;
	}

	int size = write(conn->fd, conn->out_buffer.c_str() + conn->out_pos, conn->out_buffer.size() - conn->out_pos);
	if (size < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// kernel writer buff full, keep EPOLL_OUT event
			LOG(LLD, "do_write:EAGAIN");
			return 0;
		}
		// XXX some thing go wrong, should we delete connect?
		LOG(LLA, "**************** write error, errno=%d ****************", errno);
		return -1;
	}

	conn->out_pos += size;
	// check if all out_buffer sended
	if (conn->out_buffer.size() == (size_t)conn->out_pos)
	{
		// 1.clean out_buffer
		// 2.cancel listen write
		conn->out_buffer = "";
		conn->out_pos = 0;

		sp_write(pfd, conn->fd, (void *)conn, false);
	}
	return 0;
}

int server_disconnect(connect_t *conn)
{
	// check if server is gate server
	// if true, remove from g_gate_list, and clean g_uin_map
	for (auto iter = g_gate_list.begin(); iter != g_gate_list.end(); iter++)
	{
		if (*iter == conn)
		{
			// clean client
			for (auto client_iter = g_uin_map.begin(); client_iter != g_uin_map.end();)
			{
				if (client_iter->second == conn)
				{
					auto it = client_iter;
					client_iter++;
					g_uin_map.erase(it);
				}
				else
				{
					client_iter++;
				}
			}
			g_gate_list.erase(iter);
			LOG(LLC, "server_disconnect:gate server disconnect");
			return 0;
		}
	}

	// if server is other server, remove from g_server_map
	for (auto iter = g_server_map.begin(); iter != g_server_map.end(); iter++)
	{
		if (iter->second == conn)
		{
			g_server_map.erase(iter);
			LOG(LLC, "server_disconnect:server %d disconnect", iter->first);
			return 0;
		}
	}

	LOG(LLA, "server_disconnect:unknow server disconnect!!!");

	return 0;
}

///// NETWORK END ]

///// MSG START [

int msg_read(char *in_buffer, int in_buffer_size, msg_head_t *header, char *body, int body_size, bool copy)
{
	// 1.check if has msg head
	// 2.unpack msg head
	// 3.check if has body
	// 4.check if has space for body
	// 5.memcpy data to body
	
	if (in_buffer_size < msg_head_t::MSG_HEAD_LEN)
	{
		return 0;
	}

	const char *ptr = header->unpack(in_buffer);

	if (in_buffer_size - msg_head_t::MSG_HEAD_LEN < header->body_len)
	{
		return 0;
	}

	if (header->body_len > body_size)
	{
		// body_len to big in msg head, should not happend.
		// if true, should close this connect
		return -1;
	}

	if (copy)
	{
		memcpy(body, ptr, header->body_len);
	}
	
	return msg_head_t::MSG_HEAD_LEN + header->body_len;
}

int msg_write(int pfd, connect_t *conn, msg_head_t *header, const char *body, int body_size)
{
	// 1.pack and add header 
	// 2.add data
	// 3.poll mod
	
	char header_data[msg_head_t::MSG_HEAD_LEN+1];
	header->body_len = body_size;
	header->pack(header_data);
	conn->out_buffer.append(header_data, msg_head_t::MSG_HEAD_LEN);

	if (body_size > 0)
	{
		conn->out_buffer.append(body, body_size);
	}

	sp_write(pfd, conn->fd, (void *)conn, true);

	return 0;
}

int msg_write(int pfd, unsigned int uin, msg_head_t *header, const char *body, int body_size)
{
	// 1.get bridge conn by uin
	// 2.msg write

	connect_t *conn = get_conn_by_uin(uin);
	if (conn == NULL)
	{
		LOG(LLB, "msg_write:uin not found %u", uin);
		return 0;
	}
	msg_write(pfd, conn, header, body, body_size);

	return 0;
}

int handle_msg(int pfd, connect_t *conn, msg_head_t *header, const char *body)
{
	switch (header->cmd_id)
	{
	case CMD_SERVER_REGISTER :
		// server register, add into server map
		// uin is server type
		LOG(LLD, "handle_msg:server %d register", header->uin);
		if (header->uin == SERVER_TYPE_GATE)
		{
			g_gate_list.push_back(conn);
		}
		else
		{
			auto iter = g_server_map.find(header->uin);
			if (iter != g_server_map.end())
			{
				LOG(LLA, "handle_msg:server already register %u", header->uin);
				return 0;
			}
			g_server_map.insert(std::make_pair(header->uin, conn));
		}
		break;

	case CMD_CLIENT_CONNECT :
		g_uin_map.insert(std::make_pair(header->uin, conn));
		break;

	case CMD_CLIENT_DISCONNECT :
		g_uin_map.erase(header->uin);
		break;

	default :
		// all client logic will start here

		// assume one single msg logic will not use more than 5ms
		// usleep (5 * 1000);

		// busy loop
		double t1 = get_time_ms();
		double t2 = get_time_ms();
		while (t2 - t1 < 5)
		{
			t2 = get_time_ms();
		}
		

		// send data to client
		char body_data[MAX_MSG_BODY_SIZE+1];
		int size = sprintf(body_data, "welcome to game server %lu", time(NULL));
		msg_write(pfd, header->uin, header, body_data, size);
		break;
	}

	return 0;
}

int handle_logic(connect_t *conn, int pfd)
{
	// handle msg
	msg_head_t header;
	char body[MAX_MSG_BODY_SIZE+1];

	int msg_count = 0;
	int total_size = conn->in_end_pos - conn->in_start_pos;
	while (conn->in_end_pos != conn->in_start_pos)
	{
		int ret = msg_read(conn->in_start_pos, total_size, &header, body, MAX_MSG_BODY_SIZE, true);
		if (ret < 0)
		{
			// something wrong in client, close it
			LOG(LLA, "*********** msg_read error, header.body_len=%d *********", header.body_len);
			sp_del(pfd, conn->fd);
			free_conn(conn);
			return -1;
		}
		if (ret == 0)
		{
			// no more msg
			break;
		}

		// debug print msg	
		// LOG(LLD, "handle_logic:cmd_id=%d uin=%u body_len=%d msg_index=%u\n", header.cmd_id, header.uin, header.body_len, header.msg_index);
		// body[header.body_len] = '\0';
		// LOG(LLD, "handle_logic:body=[%s]\n", body);

		// CORE LOGIC
		handle_msg(pfd, conn, &header, body);

		msg_count++;
		
		conn->in_start_pos += ret;
		total_size -= ret;
		
		if (msg_count >= 10)
		{
			break;
		}
	}

	if (total_size == 0)
	{
		// no msg need to handle
		conn->in_start_pos = conn->in_buffer;
		conn->in_end_pos = conn->in_buffer;
		return 0;
	}

	if (conn->in_end_pos == conn->in_buffer + conn->in_buffer_size)
	{
		// buffer is full and still has data not handled, memmove data to in_buffer head
		// if happended, consider upgrade connect in_buffer_size
		LOG(LLC, "**************** do memmove[%d] in_buffer_size=%d in_start_pos=%d offset=%d ****************"
		, conn->uid, conn->in_buffer_size, conn->in_start_pos - conn->in_buffer, conn->in_end_pos - conn->in_start_pos);

		memmove(conn->in_buffer, conn->in_start_pos, total_size);
		conn->in_start_pos = conn->in_buffer;
		conn->in_end_pos = conn->in_buffer + total_size;
	}
	
	if (conn->in_end_pos != conn->in_buffer)
	{
		// still has msg to handle, listen write to keep loop run
		sp_write(pfd, conn->fd, (void *)conn, true);
	}
	return 0;
}

///// MSG END ]

void print_info()
{
	static time_t last_time = time(NULL);

	time_t now = time(NULL);

	const int PRINT_INFO_TIMEOUT = 0;
	if (now - last_time < PRINT_INFO_TIMEOUT)
	{
		return;
	}
	last_time = now;

	LOG(LLD, "g_uin_map.size()=%lu", g_uin_map.size());
	for (int i = 0; i < MAX_CONNECT; i++)
	{
		connect_t *conn = g_conn_list + i;
		if (conn->is_valid())
		{
			LOG(LLD, "[%d] in_buffer_size=%d start=%d end=%d offset=%d"
			, conn->uid, conn->in_buffer_size
			, conn->in_start_pos - conn->in_buffer
			, conn->in_end_pos - conn->in_buffer
			, conn->in_end_pos - conn->in_start_pos
			);
		}
	}
}

int load_config(std::string path, bool save_log_flag)
{
	int ret = 0;
	config_reader reader;
	ret = reader.load(path.c_str());
	if (ret < 0)
	{
		return -1;
	}

	SERVER_IP = reader.get_string("SERVER_IP", "0.0.0.0");
	SERVER_PORT = reader.get_int("SERVER_PORT", 7711);
	MAX_CONNECT = reader.get_int("MAX_CONNECT", 10);

	printf("SERVER_IP=[%s] SERVER_PORT=%d MAX_CONNECT=%d\n"
	, SERVER_IP.c_str(), SERVER_PORT, MAX_CONNECT);

	std::string log_file = reader.get_string("LOG_FILE", "../log/gate_server");
	int log_level = reader.get_int("LOG_LEVEL", LLD);
	LOG_INIT(log_file.c_str(), 10000, 5, (log_manager::log_level)log_level, save_log_flag);

	return 0;
}

int do_daemon(bool is_daemon)
{
	if (!is_daemon)
	{
		return 0;
	}
	return daemon(1, 1);

	/*
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		return -1;
	}
	else if (pid)
	{
		_exit(0);
	}

	if (setsid() < 0)
	{
		return -1;
	}
	return 0;
	*/
}

int detach_process(int pfd, int max_connect)
{
	// event num every loop handle
	const int MAX_EVENTS = max_connect; // (max_connect / 10)

	int ret = 0;
	event events[MAX_EVENTS];

	while (is_running)
	{
		// 1.sp_wait
		// 2.do accept, read, write

		// debug print
		print_info();

		// 1.
		int nfds = sp_wait(pfd, events, MAX_EVENTS, WAIT_TIMEOUT);
		if (nfds == -1)
		{
			if (errno == EINTR)
			{
				continue;
			}
			LOG(LLA, "sp_wait error %d\n", errno);
			return -1;
		}

		// 2.
		for (int i = 0; i < nfds; i++)
		{
			connect_t * conn;
			conn = (connect_t *)events[i].s;

			// listen_fd
			if (conn == &g_listen_conn)
			{
				ret = do_accept(conn->fd, pfd);
				continue;
			}

			// client can read
			if (events[i].read)
			{
				do_read(conn, pfd);
			}

			// client can send
			if (events[i].write)
			{
				do_write(conn, pfd);
			}

			// handle logic
			handle_logic(conn, pfd);
		}

		// one loop end
	}

	return 0;
}

void do_clean()
{
	delete [] g_conn_list;
	sp_release(g_pfd);
}

void signal_handler(int sig)
{
	LOG(LLC, "signal get [%d]", sig);
	switch (sig)
	{
	case SIGSEGV :
		LOG(LLA, "singal_handler:g_uin_map.size()=%lu g_free_connect=%d"
		, g_uin_map.size(), g_free_connect);
		break;
	case SIGINT :
	case SIGTERM :
		is_running = false; // if false, server will shutdown
		LOG(LLC, "server will shutdown");
		break;
	}
}

int main(int argc, char **argv)
{

	// 0.do daemon
	// 1.load config
	// 2.init all connection
	// 3.init listen_fd
	// 4.init poll
	// 5.listen accept
	// 6.loop poll wait
	// 7.server down, do clean
	
	bool is_daemon = false;
	std::string config_file = "../conf/game_server.conf";

	// -d for daemon, -c for config file path
	int opt;
	while ((opt = getopt(argc, argv, "dc:")) != -1)
	{
		switch (opt)
		{
		case 'd' :
			is_daemon = true;
			break;
		case 'c' :
			config_file = std::string(optarg);
			break;
		default :
			fprintf(stderr, "Usage: %s [-d] [-c config_file]\n", argv[0]);
			exit(1);
		}
	}
	printf("is_daemon=%d config_file=%s\n", is_daemon, config_file.c_str());

	// 1.load config
	if (load_config(config_file, is_daemon) < 0)
	{
		printf("********** load_config fail\n");
		return -1;	
	}

	if (do_daemon(is_daemon) < 0)
	{
		printf("********** do_daemon fail\n");
		return -1;
	}
	
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGSEGV, signal_handler);

	// 2.init connection
	init_connection_list(MAX_CONNECT);

	// 3.init listen socket
	int listen_fd = init_listen_fd(SERVER_IP, SERVER_PORT);
	if (listen_fd < 0)
	{
		printf("********** init_listen_fd fail **********\n");
		return -1;
	}
	LOG(LLD, "main:listen_fd=%d", listen_fd);

	// 4.init poll
	g_pfd = sp_create();
	if (sp_invalid(g_pfd))
	{
		printf("********** sp_create fail %d **********\n", errno);
		return -1;
	}

	// 5
	sp_add(g_pfd, listen_fd, (void *)&g_listen_conn);

	// 6.loop and wait
	detach_process(g_pfd, MAX_CONNECT);

	// 7.server down, do clean 
	do_clean();
	
	LOG(LLC, "---- SERVER DOWN ----");

	return 0;
		
}

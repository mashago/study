#ifdef __linux__
#define _GNU_SOURCE
#endif

#if __APPLE__
#define daemon avoid_warning
#endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <syslog.h>
#include <stdarg.h>

#if __APPLE__
#undef daemon
extern int daemon(int, int);
#endif


// #define SERVER_ADDR		"192.168.1.33"
#define SERVER_ADDR		"0.0.0.0"
#define SERVER_PORT		7711

#define OUTPUT_PRINT	0
#define OUTPUT_LOG		1
static int output_type = OUTPUT_PRINT;


#define __log_buffer_size	500
static char __log_buffer[__log_buffer_size];
void print_msg(int level, const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsnprintf(__log_buffer, __log_buffer_size, fmt, args);
	va_end(args);
	if (output_type == OUTPUT_PRINT) {
		printf("%s", __log_buffer);
		return;
	} else if (output_type == OUTPUT_LOG) {
		syslog(level, "%s", __log_buffer);
		return;
	}
}


// handle dead child
void sig_chld(int sig)
{
	pid_t pid;
	int stat;
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
	{
		print_msg(LOG_INFO, "sig_chld:pid = %d\n", pid);
	}
	signal(SIGCHLD, sig_chld);
}

int main (int argc, char *argv[])
{
	int ret;
	print_msg(LOG_INFO, "hello simple_server\n");

	signal(SIGHUP, SIG_IGN);
	signal(SIGCHLD, sig_chld);

	pid_t g_pid = -1;

	int c;
	while ((c = getopt(argc, argv, "dh:p:")) != -1)
	{
		switch (c)
		{
		case 'd':
			{
				// int daemon(int nochdir, int noclose);
				// daemon() == fork() + father process exit + setsid()
				g_pid = daemon(1, 1);
				if (g_pid < 0) {
					return -1;
				}
				output_type = OUTPUT_LOG;
				break;
			}
		}
	}
	/*
	if  (argc == 1) {
		// int daemon(int nochdir, int noclose);
		// daemon() == fork() + father process exit + setsid()
		g_pid = daemon(1, 1);
		if (g_pid < 0) {
			return -1;
		}
		output_type = OUTPUT_LOG;
	}
	*/

	print_msg(LOG_INFO, "g_pid=%d\n", g_pid);

	/*
	 *  struct in_addr {
	 *  	in_addr_t		s_addr;
	 *  }
	 *
	 *  // ipv4 socket address struct
	 *  struct sockaddr_in {
	 *  	uint8_t			sin_len;
	 *  	sa_family_t		sin_family;		*
	 *  	in_port_t		sin_port;		*
	 *  	struct in_addr	sin_addr;		*
	 *  	char			sin_zero[8];
	 *  }
	 *	
	 *  // only for force cast	
	 *	struct sockaddr {
	 *		uint8_t			sa_len;
	 *		sa_family_t		sa_family;
	 *		char 			sa_data[14];
	 *	}
	 *
	 */

	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		print_msg(LOG_ERR, "socket:errno=%d\n", errno);
		return -1;
	}

	int flag = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&flag, sizeof(flag));

	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));

	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	// 3 way to init host address
	// 1. inet_addr()
	// sin.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	// 2. inet_aton()
	// inet_aton(SERVER_ADDR, &sin.sin_addr);
	// 3. inet_pton()
	inet_pton(AF_INET, SERVER_ADDR, &sin.sin_addr);

	char host_str[100];
	inet_ntop(sin.sin_family, &(sin.sin_addr), host_str, sizeof(sin)); // get ip
	int port = ntohs(sin.sin_port);
	sprintf(host_str+strlen(host_str), ":%d", port);
	print_msg(LOG_INFO, "host_str=%s\n", host_str);

	ret = bind(fd, (struct sockaddr*)&sin, sizeof(sin));
	if (ret < 0) {
		print_msg(LOG_ERR, "bind:errno=%d\n", errno);
		return -1;
	}

	ret = listen(fd, 1024);
	if (ret < 0) {
		print_msg(LOG_ERR, "listen:errno=%d\n", errno);
		return -1;
	}

	pid_t pid;
	for (;;) {

		struct sockaddr_in cin;
		bzero(&cin, sizeof(cin));
		socklen_t address_len = sizeof(cin);
		int client_fd = accept(fd, (struct sockaddr*)&cin, &address_len); 
		if (client_fd < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			print_msg(LOG_INFO, "accept error %d\n", errno);
			exit(1);
		}
		print_msg(LOG_INFO, "client_fd=%d\n", client_fd);

		if ((pid = fork()) == 0) {
			// child process
			close(fd);
			char in_buffer[100];
			int ret = read(client_fd, in_buffer, sizeof(in_buffer));
			if (ret < 0) {
				exit(0);
			}
			in_buffer[ret] = '\0';
			print_msg(LOG_INFO, "in_buffer=%s\n", in_buffer);
			close(client_fd);
			exit(0);
		}
		close(client_fd);
	
	}


	return 0;
}

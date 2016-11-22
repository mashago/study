
extern "C" {
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <syslog.h>
#include <stdarg.h>
}

#include <string>
#include "HttpSocketManager.h"

// #define SERVER_ADDR		"192.168.1.33"
#define SERVER_ADDR		"0.0.0.0"
#define SERVER_PORT		7711
#define MAX_BUFFER		5000

// handle dead child
void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
	}
}

int main (int argc, char *argv[])
{
	int ret;
	printf("hello http server\n");

	signal(SIGHUP, SIG_IGN);
	signal(SIGCHLD, sig_chld);

	pid_t g_pid = -1;
	if  (argc == 2) {
		// daemon process
		g_pid = fork();
		if (g_pid < 0) {
			return -1;
		} else if (g_pid) {
			// father process exit
			_exit(0);
		}

		// child process come here
		// setsid() creates a new session if the calling process is not a process group leader.  The calling process is the leader of the  new session (i.e., its session ID is made the same as it process ID).  The calling process also  becomes  the  process group leader of a new process group in the session
		if (setsid() < 0) {
			return(0);
		}

		// int daemon(int nochdir, int noclose);
		// daemon() == fork() + father process exit + setsid()
		/*
		g_pid = daemon(1, 1);
		if (g_pid < 0) {
			return -1;
		}
		*/
	}
	printf("g_pid=%d\n", g_pid);

	HttpSocketManager::Create();
	HttpSocketManager::GetThis()->Init(SERVER_ADDR, SERVER_PORT);

	char in_buffer[MAX_BUFFER+1];
	in_buffer[0] = '\0';
	char out_buffer[MAX_BUFFER+1];
	out_buffer[0] = '\0';

	// pid_t pid;
	for (;;) {
		
		ret = HttpSocketManager::GetThis()->Wait(0);
		if (ret == 0)
		{
			continue;
		}
		bzero(in_buffer, sizeof(in_buffer));
		HttpSocketManager::GetThis()->Recv(in_buffer, MAX_BUFFER);
		printf("---- in_buffer[%zu]=[\n%s\n]\n", strlen(in_buffer), in_buffer);

		sprintf(out_buffer, "%s", "HTTP/1.1 200 OK\r\nServer: Apache\r\n\r\n");
		HttpSocketManager::GetThis()->Send(out_buffer, strlen(out_buffer));

		sprintf(out_buffer, "%s","HELLO WORLD");
		HttpSocketManager::GetThis()->Send(out_buffer, strlen(out_buffer));

		HttpSocketManager::GetThis()->Disconnect();

	}


	return 0;
}

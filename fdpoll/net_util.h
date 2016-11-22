/*
 * net_util.h
 * for init server socket, client socket
 *
 */

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


/*
#define NU_FATAL_TAG   "\x1B[31m\x1B[4mFATAL "
#define NU_BUG_TAG     "\x1B[35m\x1B[4mBUGBUG "
#define NU_ERROR_TAG   "\x1B[31mERROR "
#define NU_WARN_TAG    "\x1B[33mWARN "
#define NU_DEBUG_TAG   "\x1B[36mDEBUG "
#define NU_INFO_TAG    "\x1B[34mINFO "
#define NU_END_TAG     "\x1B[0m\n" // clean up color code
*/

#define NU_DEBUG(...)	do { printf("\x1B[36mDEBUG "); printf(__VA_ARGS__); printf("\x1B[0m\n"); fflush(stdout); } while(0)
#define NU_ERROR(...)	do { printf("\x1B[31mERROR "); printf(__VA_ARGS__); printf("\x1B[0m\n"); fflush(stdout); } while(0)

static int __socket_set_nonblock(int fd);
static int INIT_SERVER_SOCKET(const char *ip, const int port, const int nonblock);
static int INIT_CLIENT_SOCKET(const int fd, const int nonblock);

static inline int __socket_set_nonblock(int fd)
{
	int flag;
	// set no delay
	flag = fcntl(fd, F_GETFL, 0);
	if (flag < 0) {
		NU_ERROR("__socket_set_nonblock:getfl %d\n", errno);
		return -1;
	}

	// O_NONBLOCK is mostly same as O_NDELAY, are make io become non-blocking
	// but O_NDELAY will return 0 when io get nothing
	// , O_NONBLOCK will return -1 when io get nothing.
	// in gnu c, O_NDELAY is use a O_NONBLOCK marco
	// O_NONBLOCK   Non-blocking I/O; if no data is available to a read call, or if a write operation would block, the read or write call returns -1 with the error EAGAIN.
	flag = fcntl(fd, F_SETFL, flag | O_NONBLOCK);
	if (flag < 0) {
		NU_ERROR("__socket_set_nonblock:setfl_nonblock %d\n", errno);
		return -1;
	}
	return 0;
}

// return main_fd
static inline int INIT_SERVER_SOCKET(const char *ip, const int port, const int nonblock)
{
	int ret;	
	int fd;
	int flag;
	int backlog = 1024;
	struct sockaddr_in sin; // in "netinet/in.h"

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

	// int socket(int domain, int type, int protocol); // protocol usually be 0
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		NU_ERROR("INIT_SERVER_SOCKET:socket %d\n", errno);
		return -1;
	}

	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	// 3 ways to set sin_addr
	// sin.sin_addr.s_addr = htonl(INADDR_ANY); // use htonl(), 
	// sin.sin_addr.s_addr = inet_addr(ip); // out date method
	// inet_aton(ip, &sin.sin_addr); // new method, not need update s_addr in sin_addr
	inet_aton(ip, &sin.sin_addr); // new method, c char* address to binary numeric
	sin.sin_port = htons(port);

	flag = 1;
	// int setsockopt(int sockfd,int level,int optname,const void *optval,socklen_t optlen);
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&flag, sizeof(flag));
	if (ret != 0) {
		NU_ERROR("INIT_SERVER_SOCKET:set_reuseaddr %d\n", errno);
		return -1;	
	}

	// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	ret = bind(fd, (struct sockaddr*)&sin, sizeof(sin));
	if (ret != 0) {
		NU_ERROR("INIT_SERVER_SOCKET:bind %d\n", errno);
		return -1;	
	}

	if (nonblock) {
		ret = __socket_set_nonblock(fd);
		if (ret != 0) {
			return -1;	
		}
	}

	// int listen(int sockfd, int backlog);
	ret = listen(fd, backlog);
	if (ret != 0) {
		NU_ERROR("INIT_SERVER_SOCKET:listen %d\n", errno);
		return -1;	
	}

	NU_DEBUG("INIT_SERVER_SOCKET:fd=%d", fd);

	return fd;
}

static inline int INIT_CLIENT_SOCKET(const int fd, const int nonblock)
{
	int flag = 1;
	int ret;
	// note: TCP_NODELAY in <netinet/tcp.h>
	ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
	if (ret != 0) {
		NU_ERROR("INIT_CLIENT_SOCKET:set_tcp_nodelay %d\n", errno);
		return -1;
	}

#ifdef __linux__
    // keepalive
    flag = 1;
    ret = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(int));
	if (ret < 0) {
    	NU_ERROR("INIT_CLIENT_SOCKET:setsockopt_keepalive %d\n", errno);
		return -1;
	}

    flag = 5; 
	if (ret < 0) {
    ret = setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &flag, sizeof(int));
    	NU_ERROR("INIT_CLIENT_SOCKET:setsockopt_keepcnt %d\n", errno);
		return -1;
	}
    flag = 5;  // after 5 seconds of idle, start keepalive
	if (ret < 0) {
    ret = setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, &flag, sizeof(int));
    	NU_ERROR("INIT_CLIENT_SOCKET:setsockopt_keepidle %d\n", errno);
		return -1;
	}

    flag = 5;  // interval is 5 seconds
	if (ret < 0) {
    ret = setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &flag, sizeof(int));
    	NU_ERROR("INIT_CLIENT_SOCKET:setsockopt_keepintvl %d\n", errno);
		return -1;
	}
#endif

	// FD_CLOEXEC: close-on-exec, WHY USE HERE???
	fcntl(fd, F_SETFD, FD_CLOEXEC);

	if (nonblock) {
		ret = __socket_set_nonblock(fd);
		if (ret != 0) {
			return -1;	
		}
	}

	ret = 0;
	return ret;
}


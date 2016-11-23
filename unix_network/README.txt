1. simple_server.c
	fork()
	int fdstdin = fileno(stdin);

2. exec_test.c

3. select_client.c
	fd_set rset;
	FD_ZERO(fd_set *);
	FD_SET(fd, fd_set *);
	FD_CLR(fd, fd_set *);
	FD_ISSET(fd, fd_set *);
	num_ready = select(nfds, fd_set *read_set, fd_set *write_set, fd_set *error_set, timeval *timeout);

4. poll_client.c
	struct pollfd {
		int fd;
		short events;
		short revents;
	}
	events,revents:POLLIN, POLLOUT, POLLERR, POLLHUP, POLLNVAL
	struct pollfd pollfds[MAX_FD];
	pollfds[i].fd=fd;
	pollfds[i].events=POLLIN/POLLOUT;
	num_ready = poll(pollfds, i+1, int timeout);
	if (pollfds[i].revents & POLLIN/POLLOUT)

5. dns_test.c
	struct hostent {
		char	*h_name;
		char 	**h_aliases;
		int		h_addrtype;
		int 	h_length;
		char 	**h_addr_list; // ptr to array to ipv4 address numeric
	}
	struct hostent * = gethostbyname(const char *);

6. syslog_test.c
	// config in /etc/rsyslog.conf
	// log in /var/log/message
	openlog(const char *ident, int options, int facility);
	syslog(int priority, const char *message, ...);
	closelog();

7. usock_test.c
	define UNIX_DOMAIN "/tmp/xxxx" 
	fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	unlink(UNIX_DOMAIN);
	struct sockaddr_un sun;
	sun.sun_family = AF_LOCAL;
	strcpy(sun.sun_path, UNIX_DOMAIN);
	bind(), listen() ...
	connect() ...

8. usock_server.c usock_client.c

9. epoll_client.c
	int epollfd = epoll_create(int size);
	int epoll_ctl(int epollfd, int op, int fd, struct epoll_event *)
	op: EPOLL_CTL_ADD, EPOLL_CTL_MOD, EPOLL_CTL_DEL
	typedef union epoll_data {
		void		*ptr;
		int			fd;
		__uint32_t	u32;
		__uint64_t	u64;
	} epoll_data_t
	struct epoll_event {
		__uint32_t		events;
		epoll_data_t	data;
	}
	epoll_event.events:EPOLLIN,EPOLLOUT,EPOLLERR,EPOLLHUP,EPOLLLT,EPOLLET
	num_ready = epoll_wait(epollfd, events_array, max_events, timout);
	
10. pthread_test.c
	pthread_attr_t attr;
	int pthread_attr_init(pthread_attr_t *attr);

	int pthread_create(pthread_t *tid, const pthread_attr_t *attr
	, void * (*func)(void *), void *arg);
	// status is pthread *func return 
	int pthread_join(pthread_t *tid, void **status); 
	// get self pthread_t
	pthread_t pthread_self(void); 
	// set thread cannot be join
	int pthread_detach(pthread_t tid);
	void pthread_exit(void **status);

	pthread_mutexattr_t attr;  
	int pthread_mutexattr_init(pthread_mutexattr_t *attr);  
	int pthread_mutexattr_setpshared(pthread_mutexattr_t *, int pshared); 
	pshared: PTHREAD_PROCESS_SHARED, PTHREAD_PROCESS_PRIVATE (default)

	pthread_mutex_t lock;
	int pthread_mutex_init(pthread_mutex_t *, pthread_mutexattr_t *);
	int pthread_mutex_lock (pthread_mutex_t *);
	int pthread_mutex_unlock (pthread_mutex_t *);

	pthread_condattr_t attr;
	pthread_cond_t cond;
	int pthread_cond_init(pthread_cond_t *, pthread_condattr *);
	int pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
	int pthread_cond_signal(pthread_cond_t *);
	int pthread_cond_broadcast(pthread_cond_t *);


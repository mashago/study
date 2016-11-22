
#define FD_READ		0
#define FD_WRITE	1	

int fdpoll_init(void);

int fdpoll(long timeout_msecs);

// get active client data
void * fdpoll_get(void);

void fdpoll_add(int fd, void *client_data, int rw);

void fdpoll_del(int fd);

// void fdpoll_change(int fd, void *client_data, int rw);

int fdpoll_check(int fd);


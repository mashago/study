

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <poll.h>
#include "fdpoll.h"

// total fd num
static int nfds; 
// 
static int * fd_rw;
static void ** fd_data;
static int nreturned; // WATCH return active fd num
static int next_ridx; // next active fd ridx in xpoll_rfdidx list

////////////////////////// poll member
static struct pollfd* pollfds; // pollfd list
static int npoll_fds; // num of poll_fds
static int *poll_fdidx; // fd to pollfds list index list
static int *poll_rfdidx; // ridx to active fd list
static int poll_init(int fd_nums);
static int poll_watch(long timeout_msecs);
static void poll_add(int fd, int rw);
static void poll_del(int fd);
static int poll_get(int ridx);
static int poll_check(int fd);

#define INIT(fd_nums)			poll_init(fd_nums)
#define WATCH(ms)			poll_watch(ms)
#define ADD_FD(fd, rw)		poll_add(fd, rw)
#define DEL_FD(fd)			poll_del(fd)
#define GET_FD(ridx)		poll_get(ridx)
#define CHECK_FD(ridx)		poll_check(fd)
////////////////////////////////// 

/*
int <sys/resource.h>
struct rlimit {
	rlim_t rlim_cur;
	rlim_t rlim_max;
};
*/

static int get_max_nfds(void)
{
	int m;
#ifdef RLIMIT_NOFILE
	struct rlimit rl;
#endif
	// get max count fd this program can get
	m = getdtablesize();
#ifdef RLIMIT_NOFILE
	if (getrlimit(RLIMIT_NOFILE, &rl) == 0) {
		// printf("rl.rlim_cur=%llu rl.rlim_max=%llu\n", rl.rlim_cur, rl.rlim_max);
		m = rl.rlim_cur;
		if (rl.rlim_max == RLIM_INFINITY) {
			// reset max count fd we can use in this program
			rl.rlim_cur = 8192;
		} else if (rl.rlim_max > rl.rlim_cur) {
			rl.rlim_cur = rl.rlim_max;
		}
		if (setrlimit(RLIMIT_NOFILE, &rl) == 0) {
			m = rl.rlim_cur;
		}
	}
#endif
	return m;
}

int fdpoll_init(void)
{

	nfds = get_max_nfds();

	fd_rw = (int*) malloc(sizeof(int) * nfds);
	fd_data = (void**) malloc(sizeof(void*) * nfds);
	if (fd_rw == (int*) 0 || fd_data == (void **) 0) {
		return -1;
	}
	for (int i=0; i<nfds; i++) {
		fd_rw[i] = -1;
	}
	if (INIT(nfds) == -1) {
		return -1;
	}
	return nfds;
}

int fdpoll(long timeout_msecs)
{
	nreturned = WATCH(timeout_msecs);
	next_ridx = 0;
	return nreturned;
}

void fdpoll_add(int fd, void *data, int rw)
{
	if (fd < 0 || fd >= nfds || fd_rw[fd] != -1) {
		return;
	}
	ADD_FD(fd, rw);
	fd_rw[fd] = rw;
	fd_data[fd] = data;
}

void fdpoll_del(int fd)
{
	if (fd < 0 || fd >= nfds || fd_rw[fd] == -1) {
		return;
	}
	DEL_FD(fd);
	fd_rw[fd] = -1;
	fd_data[fd] = (void*) 0;
}

void * fdpoll_get(void)
{
	int fd;
	if (next_ridx >= nreturned) {
		return (void*) -1; // no more active fd
	}
	fd = GET_FD(next_ridx++);
	if (fd < 0 || fd >= nfds) {
		return (void*) 0;
	}
	return fd_data[fd];
}

int fdpoll_check(int fd)
{
	if (fd < 0 || fd >= nfds || fd_rw[fd] == -1) {
		return 0;
	}
	return CHECK_FD(fd);
}


///////////////// POLL START ///////////////

static int poll_init(int fd_nums)
{
	pollfds = (struct pollfd*) malloc(sizeof(struct pollfd) * fd_nums);
	poll_fdidx = (int*) malloc(sizeof(int) * fd_nums);
	poll_rfdidx = (int*) malloc(sizeof(int) * fd_nums);
	if (pollfds == (struct pollfd*) 0 || poll_fdidx == (int*) 0
	|| poll_rfdidx == (int*) 0) {
		return -1;
	}
	for (int i=0; i<fd_nums; i++) {
		pollfds[i].fd = poll_fdidx[i] = -1;
	}
	return  0;
}

static int poll_watch(long timeout_msecs)
{
	int num, ridx;
	num = poll(pollfds, npoll_fds, (int) timeout_msecs);
	if (num <= 0) {
		return num;
	}
	ridx = 0;
	for (int i=0; i<npoll_fds; ++i) {
		if (pollfds[i].revents 
		& (POLLIN | POLLOUT | POLLERR | POLLHUP | POLLNVAL)) {
			// add active fd to poll_rfdidx list
			poll_rfdidx[ridx++] = pollfds[i].fd; 
		}
		if (ridx == num) {
			break;
		}
	}
	return ridx; // should be equeal to num
}

static void poll_add(int fd, int rw)
{
	if (npoll_fds >= nfds) {
		return;
	}
	pollfds[npoll_fds].fd = fd;
	switch (rw) {
	case FD_READ: pollfds[npoll_fds].events = POLLIN; break;
	case FD_WRITE: pollfds[npoll_fds].events = POLLOUT; break;
	default: break;
	}
	poll_fdidx[fd] = npoll_fds;
	++npoll_fds;
}

static void poll_del(int fd)
{
	int idx = poll_fdidx[fd];

	if (idx < 0 || idx >= nfds) {
		return;
	}
	--npoll_fds;
	// copy last pollfd to del pollfd
	pollfds[idx] = pollfds[npoll_fds];
	poll_fdidx[pollfds[idx].fd] = idx;

	// clean last pollfd
	pollfds[npoll_fds].fd = -1;
	poll_rfdidx[fd] = -1;
}

static int poll_get(int ridx)
{
	if (ridx < 0 || ridx >= nfds) {
		return -1;
	}
	return poll_rfdidx[ridx];
}

static int poll_check(int fd)
{
	int fdidx = poll_fdidx[fd];
	if (fdidx < 0 || fdidx >= nfds) {
		return 0;
	}

	if (pollfds[fdidx].revents & POLLERR) {
		return 0;
	}

	switch (fd_rw[fd]) {
	case FD_READ: return pollfds[fdidx].revents & (POLLIN | POLLHUP | POLLNVAL);
	case FD_WRITE: return pollfds[fdidx].revents & (POLLOUT | POLLHUP | POLLNVAL);
	default: return 0;
	}
}

///////////////// POLL END ///////////////


1. pipo_test.c
	no name pipe:
	// fds[0] for read, fds[1] for write
	int pipe(int fds[2]);

	// command is a shell command, type is "r" or "w"
	FILE * popen(const char * command, const char *type);
	int pclose(FILE *fp);

2. fifo_test.c
	int mkfifo(FIFO_PATH, FILE_MODE); // mkfifo includ O_CREATE | O_EXCL

3. sem_test.c
	// mode and value are optional
	sem_t * sem_open(const char *name, int oflag, int mode, int value);

	// semaphore will not remove until it closed
	int sem_unlink(const char *name);

	int sem_wait(sem_t *sem);
	int sem_post(sem_t *sem);

	int sem_close(sem_t *sem)

4. mmap.c
	mmap:
	fd = open(path, ...); // open a file
	void * mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
	// or open a unname mmap, fd = -1, flags add MAP_ANON
	int *ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

	// addr is mmap() return 
	munmap(void *addr, size_t len); 

	int msync(void * addr, size_t len, int flags); 

	share memory:
	int shm_open(const char *name, int oflag, mode_t mode);
	ftruncate(int fd, off_t length); // resize shm size
	mmap(...)
	shm_unlink(const char *name);


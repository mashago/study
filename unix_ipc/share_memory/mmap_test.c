#ifdef __linux__
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define SEM_PATH	"/tmp/sem.2"
#define SEM_FLAG	(O_CREAT | O_EXCL)
#define FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define FILE_PATH	"/tmp/mmap.2"

#define SHM_PATH	"/tmp/shm.3"

/*
 * share memory test
 */ 

/*
 * mmap:
 * fd = open(path, ...);
 * void * mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
 *
 * munmap(void *addr, size_t len); 
 * addr is mmap() return 
 * 
 * int msync(void * addr, size_t len, int flags); 
 *
 * posix share memory:
 * int shm_open(const char *name, int oflag, mode_t mode);
 * ftruncate(int fd, off_t length); // resize shm size
 * mmap(...)
 * shm_unlink(const char *name);
 */

// open a normal file, mmap it
int * mmap_normal()
{
	printf("mmap_normal()\n");
	int value = 0;
	int fd = open(FILE_PATH, O_RDWR | O_CREAT, FILE_MODE);
	write(fd, &value, sizeof(value));
	
	int *ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	*ptr = 10;
	return ptr;
}

// no name mmap, flags=MAP_SHARED | MAP_ANON, fd = -1
int * mmap_noname()
{
	printf("mmap_noname()\n");
	int *ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

	*ptr = 10;
	return ptr;
}

// /dev/zero mmap, not work in macosx
int * mmap_zero()
{
	printf("mmap_zero()\n");
	
	int value = 0;
	int fd = open("dev/zero", O_RDWR); 
	write(fd, &value, sizeof(value));

	int *ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	return ptr;
}

int * share_memory()
{
	printf("share_momory()\n");

	// POSIX share memory
	int value = 0;
	shm_unlink(SHM_PATH);
	int fd = shm_open(SHM_PATH, O_RDWR | O_CREAT, FILE_MODE);
	ftruncate(fd, sizeof(value)); // resize share memory size

	int *ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	*ptr = 10;

	return ptr;
}

int * my_mmap()
{
	
	// return mmap_normal();
	// return mmap_noname();
	// return mmap_zero();
	return share_memory();
}

int main(int argc, char **argv)
{
	printf("mmap_test\n");

	sem_t * sem;
	sem = sem_open(SEM_PATH, SEM_FLAG, FILE_MODE, 1);
	if (sem == SEM_FAILED) {
		printf("sem_open:errno=%d\n", errno);
		exit(0);
	}
	sem_unlink(SEM_PATH);

	/*
	int value = 0;
	int fd = open(FILE_PATH, O_RDWR | O_CREAT, FILE_MODE);
	write(fd, &value, sizeof(value));
	
	int *ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);
	*/

	int *ptr = my_mmap();

	if (fork() == 0) {
		for (int i=0; i<10; i++) {
			sem_wait(sem);
			printf("child v=%d\n", (*ptr)++);
			sem_post(sem);
		}
		exit(0);
	}

	for (int i=0; i<10; i++) {
		sem_wait(sem);
		printf("father v=%d\n", (*ptr)++);
		sem_post(sem);
	}

	int stat;
	wait(&stat);

	munmap(ptr, sizeof(int));

	return 0;
}

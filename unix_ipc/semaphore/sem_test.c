/*
 * POSIX semaphore test
 */ 
#include <stdio.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#ifdef __linux__
#define SEM_PATH	"semfile.1"
#else
#define SEM_PATH	"/tmp/semfile.1"
#endif

#define SEM_FLAG	(O_CREAT | O_EXCL)
#define FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
 *
 * sem_t * sem_open(const char *name, int oflag, mode_t mode, int value);
 * mode and value are optional
 * errno == ENOENT
 * 这是由于在内核中，创建信号量的默认路径是/dev/shm。当你要创建一个信号量/tmp/mysem时，实际上是创建了一个/dev/shm /sem.tmp/mysem，而这里由于/dev/shm/tmp目录根本就不存在，所以会出错。
 * 解决方法：
 *     直接写信号量文件的名字或宏定义不带路径，将会创建在/dev/shm中：sem_open("mysem", ...) 
 *
 * int sem_unlink(const char *name);
 * semaphore will not remove until it closed
 *
 * int sem_wait(sem_t *sem);
 * int sem_post(sem_t *sem);
 *
 * int sem_close(sem_t *sem)
 *
 */

int main(int argc, char **argv)
{
	printf("semaphore_test\n");


	sem_t *sem;
	sem = sem_open(SEM_PATH, SEM_FLAG, FILE_MODE, 1);
	if (sem == SEM_FAILED) {
		printf("sem_open:errno=%d\n", errno);
		exit(0);
	}
	sem_unlink(SEM_PATH); // delete path, but not effect opened semaphore

#ifdef __linux__
	// Note that in Mac OS X Yosemite (10.10) at least, sem_init(), sem_destroy() and sem_getvalue() are marked 'deprecated' and generate compiler warnings as a result.
	int ret;
	int value;
	ret = sem_getvalue(sem, &value);
	if (ret != 0) {
		printf("sem_getvalue:errno=%d\n", errno);
		exit(0);
	}
	printf("before sem_wait, sem value=%d\n", value);
#endif

	printf("father sem_wait() start\n");
	sem_wait(sem); // reduce sem
	printf("father sem_wait() finish\n");

#ifdef __linux__
	ret = sem_getvalue(sem, &value);
	if (ret != 0) {
		printf("sem_getvalue:errno=%d\n", errno);
		exit(0);
	}
	printf("after sem_wait, sem value=%d\n", value);
#endif

	pid_t pid;
	if ((pid = fork()) == 0) {
		printf("child sem_wait() start\n");
		sem_wait(sem);
		printf("child sem_wait() finish\n");
		sem_post(sem);
		exit(0);
	}
	sem_post(sem);

	sleep(3);	

	sem_close(sem);

	return 0;
};

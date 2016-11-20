
#include <stdio.h>
#include <stdlib.h> // for exit()
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h> // for unlink()
#include <string.h>
#include <pthread.h>

#define UNIX_DOMAIN	"/tmp/unix_tmp.sock"

int init_server()
{
	int ret;
	int fd;
	struct sockaddr_un sun;
	memset(&sun, 0, sizeof(sun));
	/*
	// unix sock
	struct sockaddr_un {
		sa_family_t sun_family;
		char sun_path[UNIX_PATH_MAX];
	};
	*/

	// init sockaddr_un
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path, UNIX_DOMAIN);
	unlink(UNIX_DOMAIN); // clean up old link

	// init socket
	fd = socket(sun.sun_family, SOCK_STREAM, 0);
	printf("init_server:fd=%d\n", fd);
	if (fd <= 0) {
		exit(-6);
	}

	ret = bind(fd, (struct sockaddr*)&sun, sizeof(sun));
	if (ret != 0) {
		printf("init_server:bind_error %d\n", ret);
		exit(ret);
	}

	ret = listen(fd, 1); // for 1 unix_socket, 1 is enough
	if (ret != 0) {
		printf("init_server:listen_error %d\n", ret);
		exit(ret);
	}

	return fd;
}

int init_client()
{
	int ret;
	int fd;
	struct sockaddr_un sun;
	memset(&sun, 0, sizeof(sun));

	// init sockaddr_un
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path, UNIX_DOMAIN);

	// init socket
	fd = socket(PF_UNIX, SOCK_STREAM, 0);
	printf("init_client:fd=%d\n", fd);
	if (fd <= 0) {
		exit(-6);
	}

	ret = connect(fd, (struct sockaddr*)&sun, sizeof(sun));
	printf("init_client:connect_ret=%d\n", ret);
	if (ret < 0) {
		exit(ret);
	}

	return fd;
}

void * do_read(void *ptr)
{
	int fd = (int)(size_t)ptr;
	if (fd <= 0) {
		printf("do_read:fd_error %d", fd);
		exit(-6);
	}

	char buffer[121];
	int size;
	while ((size = read(fd, buffer, 120)) > 0) {
		buffer[size] = '\0';
		printf("do_read:buffer=%s\n", buffer);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	printf("socket_unix_start\n");

	int ret;
	int fd;
	int c_fd;
	int accept_fd;
	fd = init_server();
	c_fd = init_client();

	struct sockaddr_un sun;
	socklen_t len;
	len = sizeof(sun);
	accept_fd = accept(fd, (struct sockaddr*)&sun, &len);
	printf("fd=%d c_fd=%d accept=%d\n", fd, c_fd, accept_fd);

	pthread_t read_thread;

	ret = pthread_create(&read_thread, NULL, do_read, (void *)(size_t)(c_fd));


	char buffer[101];
	while (NULL != fgets(buffer, 100, stdin)) {

		int size;
		size = write(accept_fd, buffer, strlen(buffer));
		if (size == 0) {
			printf("client:write_size==0\n");
			return 0;
		}
		if (size < 0) {
			printf("client:write_size<0\n");
			return 0;
		}
		printf("unix_server:size=%d buffer=%s\n", size, buffer);
	}

	return 0;
}

default: platform simple_server exec_test select_client poll_client dns_test syslog_test usock_test usock_server usock_client pthread_test pthread_test2 pthread_test3 udp_server udp_client fork_client epoll_client epoll_server

OS:=$(shell uname -s)
SRC:=$(wildcard *.cpp) $(wildcard *.c)
OBJ:=$(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(SRC)) )

CFLAG:= -Wall -std=c99

platform:
	@echo Platform: $(OS)
	@echo SRC: $(SRC)
	@echo OBJ: $(OBJ)

clean:
	rm -rf simple_server
	rm -rf exec_test
	rm -rf select_client
	rm -rf poll_client
	rm -rf dns_test
	rm -rf dns_test.dSYM
	rm -rf syslog_test
	rm -rf usock_test
	rm -rf usock_server
	rm -rf usock_client
	rm -rf epoll_client
	rm -rf epoll_server
	rm -rf pthread_test
	rm -rf pthread_test2
	rm -rf udp_server
	rm -rf udp_client
	rm -rf fork_client

simple_server: simple_server.c
	gcc $(CFLAG) -o simple_server simple_server.c

exec_test: exec_test.c
	gcc $(CFLAG) -o exec_test exec_test.c

select_client: select_client.c
	gcc $(CFLAG) -o select_client select_client.c

poll_client: poll_client.c
	gcc $(CFLAG) -o poll_client poll_client.c

dns_test: dns_test.c
	gcc $(CFLAG) -o dns_test dns_test.c

syslog_test: syslog_test.c
	gcc $(CFLAG) -o syslog_test syslog_test.c

usock_test: usock_test.c
	gcc $(CFLAG) -o usock_test usock_test.c

usock_server: usock_server.c
	gcc $(CFLAG) -o usock_server usock_server.c

usock_client: usock_client.c
	gcc $(CFLAG) -o usock_client usock_client.c

epoll_client: epoll_client.c
	gcc $(CFLAG) -o epoll_client epoll_client.c

epoll_server: epoll_server.c
	gcc $(CFLAG) -o epoll_server epoll_server.c

pthread_test:	pthread_test.c
	gcc $(CFLAG) -o pthread_test pthread_test.c -lpthread

pthread_test2:	pthread_test2.c
	gcc $(CFLAG) -o pthread_test2 pthread_test2.c -lpthread

pthread_test3:	pthread_test3.c
	gcc $(CFLAG) -o pthread_test3 pthread_test3.c -lpthread

udp_server: udp_server.c
	gcc $(CFLAG) -o udp_server udp_server.c

udp_client: udp_client.c
	gcc $(CFLAG) -o udp_client udp_client.c

fork_client: fork_client.c
	gcc $(CFLAG) -o $@ $^

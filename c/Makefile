
default:	platform test ppp server client usock stu et alpha write_file locker check_cr clib

CC:=gcc
CFLAG:=-std=c99

OS:=$(shell uname -s)
INC_MYSQL=-I /usr/local/include/mysql -I /usr/include/mysql
LIB_MYSQL=-lmysqlclient

platform:
	@echo Platform: $(OS)

clean: 
	rm -rf test
	rm -rf ppp
	rm -rf server
	rm -rf client
	rm -rf stu
	rm -rf stu.o
	rm -rf alpha
	rm -rf et
	rm -rf usock
	rm -rf write_file
	rm -rf locker
	rm -rf check_cr
	rm -rf clib
	rm -rf *.dSYM

test:	test.c
	$(CC) $(CFLAG) -o $@ $^ -lm

ppp:	pthread_test.c
	$(CC) $(CFLAG) -o $@ $^ -lpthread

server:	socket_server.c
	$(CC) $(CFLAG) -o $@ $^

client:	socket_client.c
	$(CC) $(CFLAG) -o $@ $^

usock:	socket_unix.c
	$(CC) $(CFLAG) -o $@ $^ -lpthread

stu:	stu.c
	$(CC) $(CFLAG) -o $@ $^ -lm

two:	stu.c empty.c
	$(CC) $(CFLAG) -o $@ $^

obj:	stu.c empty.c
	$(CC) $(CFLAG) -c $^

et:		extern_test1.c extern_test2.c
	$(CC) $(CFLAG) -o $@ $^

alpha:		alpha_list.h alpha_list.c alpha_list_test.c
	$(CC) $(CFLAG) -o $@ $^

write_file:		write_file.c
	$(CC) $(CFLAG) -o $@ $^

locker:		locker.c
	$(CC) $(CFLAG) -o $@ $^ -pthread

check_cr:		check_cr.c
	$(CC) $(CFLAG) -o $@ $< 

clib: 	clib.c
	$(CC) $(CFLAG) -o $@ $< 

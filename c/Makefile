
default:	platform test ppp server client usock stu et alpha write_file locker check_cr

CFLAG:= -Wall -std=c99

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
	rm -rf *.dSYM

test:	test.c
	gcc $(CFLAG) -o test test.c -lm

ppp:	pthread_test.c
	gcc $(CFLAG) -o ppp pthread_test.c -lpthread

server:	socket_server.c
	gcc $(CFLAG) -o server socket_server.c

client:	socket_client.c
	gcc $(CFLAG) -o client socket_client.c

usock:	socket_unix.c
	gcc $(CFLAG) -o usock socket_unix.c -lpthread

stu:	stu.c
	gcc $(CFLAG) -o stu stu.c

two:	stu.c empty.c
	gcc $(CFLAG) -o stu stu.c empty.c

obj:	stu.c empty.c
	gcc $(CFLAG) -c stu.c empty.c

et:		extern_test1.c extern_test2.c
	gcc $(CFLAG) -o et extern_test1.c extern_test2.c

alpha:		alpha_list.h alpha_list.c alpha_list_test.c
	gcc $(CFLAG) -o alpha alpha_list_test.c alpha_list.c

write_file:		write_file.c
	gcc $(CFLAG) -o write_file write_file.c

locker:		locker.c
	gcc $(CFLAG) -o locker locker.c -pthread

check_cr:		check_cr.c
	gcc $(CFLAG) -o $@ $< 


default:	platform cserver dbio


OS:=$(shell uname -s)
## INC_MYSQL=-I /usr/local/include/mysql -I /usr/include/mysql
## LIB_MYSQL=libmysqlclient.a
FLAGS_WATCH:=BUG
INC_MYSQL=-Imysql-connector-c-osx/include
LIB_MYSQL=-Lmysql-connector-c-osx/lib
## LIB_MYSQL=libmysqlclient.a

platform:
	@echo Platform: $(OS)

ifeq "$(OS)" "Linux"
FLAGS_WATCH:=-DHAVE_SYS_POLL_H=1 -DHAVE_POLL=1
endif

ifeq "$(OS)" "Darwin"
## FLAGS_WATCH:=-DHAVE_SYS_EVENT_H=1 -DHAVE_KQUEUE=1
FLAGS_WATCH:=-DHAVE_SYS_POLL_H=1 -DHAVE_POLL=1
endif

clean: 
	rm -rf cs
	rm -rf dbio
	rm -rf fdwatch.o

fdwatch.o:		fdwatch.c
	gcc $(FLAGS_WATCH) -c fdwatch.c

cserver:		cserver.c fdwatch.o
	gcc -Wall -o cserver cserver.c fdwatch.o

dbio:			dbio.c
	gcc -Wall -o dbio -DTTT $(INC_MYSQL) dbio.c -lmysqlclient $(LIB_MYSQL)


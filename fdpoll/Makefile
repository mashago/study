
default: platform fdpoll.o ss

OS:=$(shell uname -s)
RC:=$(wildcard *.cpp) $(wildcard *.c)
OBJ:=$(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(SRC)) )


platform:
	@echo Platform: $(OS)
	@echo RC: $(RC)
	@echo OBJ: $(OBJ)


fdpoll.o: fdpoll.h fdpoll.c
	gcc -Wall -c fdpoll.c

ss: fdpoll.o ss.c net_util.h
	gcc -Wall -o ss ss.c fdpoll.o


.PHONY: clean
clean:
	rm -rf fdpoll.o
	rm -rf ss

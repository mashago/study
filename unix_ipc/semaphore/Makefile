default: platform sem_test sem_test2 sem_test3

OS:=$(shell uname -s)

CFLAG:=-Wall -std=c99
LIB:=

ifeq "$(OS)" "Linux"
LIB:=-lrt -pthread
endif

ifeq "$(OS)" "Darwin"
LIB:=-pthread
endif


platform:
	@echo Platform: $(OS)

clean:
	rm -rf sem_test

sem_test: sem_test.c
	gcc $(CFLAG) -o sem_test sem_test.c $(LIB)

sem_test2: sem_test2.c
	gcc $(CFLAG) -o sem_test2 sem_test2.c $(LIB)

sem_test3: sem_test3.c
	gcc $(CFLAG) -o sem_test3 sem_test3.c $(LIB)

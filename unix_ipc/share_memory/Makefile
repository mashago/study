default: platform mmap_test

OS:=$(shell uname -s)

LIB:=

ifeq "$(OS)" "Linux"
LIB:=-lrt
endif

ifeq "$(OS)" "Darwin"
endif

platform:
	@echo Platform: $(OS)

clean:
	rm -rf mmap_test



mmap_test: mmap_test.c
	gcc -Wall -std=c99 -o mmap_test mmap_test.c $(LIB)

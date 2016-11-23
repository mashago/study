all: platform test 

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf test

CFLAGS:=-Wall -std=c++0x -g

## make DDD=1
ifdef DDD
CFLAGS+=-DDEBUG_FLAG
endif

	
test: test.cpp
	g++ $(CFLAGS) -o test test.cpp	
	

all: platform test 

OS:=$(shell uname -s)

SRC:=$(wildcard *.cpp ../cplus/*.cpp)
NOTDIR_SRC:=$(notdir $(SRC))

platform:
	@echo Platform: $(OS)

info:
	@echo SRC: $(SRC)
	@echo NOTDIR_SRC: $(NOTDIR_SRC)


# = is set last value
x1=foo
y1=$(x1) bar
x1=xyz

# := is set current value
x2:=foo
y2:=$(x2) bar
x2:=xyz

test1:
	@echo y1: $(y1) y2: $(y2)

clean:
	rm -rf test

CFLAGS:=-Wall -std=c++0x -g

## make DDD=1
ifdef DDD
CFLAGS+=-DDEBUG_FLAG
endif

	
test: test.cpp
	g++ $(CFLAGS) -o test test.cpp	
	

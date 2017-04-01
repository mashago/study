
default: platform test



platform:
	@echo Platform: $(OS)

clean:
	rm -rf test

OS:=$(shell uname -s)

ifeq "$(OS)" "Linux"
LIB_JSON:=./lib/linux/libjson.a
INC:=-I./c++0x/ -I.
endif

ifeq "$(OS)" "Darwin"
LIB_JSON:=./lib/darwin/libjson_darwin.a
INC:=-I./c++11/ -I.
endif

test: test.cpp
	g++ -Wall --std=c++0x $(INC) -o test test.cpp $(LIB_JSON)

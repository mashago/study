all: platform epoll_lua_server 

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf epoll_lua_server	
	
epoll_lua_server: epoll_lua_server.cpp
	g++ -Wall -std=c++0x -g -o $@ $<

all: platform epoll_server 

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf epoll_server	
	
epoll_server: epoll_server.cpp
	g++ -Wall -std=c++0x -g -o epoll_server epoll_server.cpp	

all: platform epoll_server epoll_client

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf epoll_server
	rm -rf epoll_client
	
epoll_server: epoll_server.cpp
	g++ -Wall -std=c++0x -g -o epoll_server epoll_server.cpp	
	
epoll_client: epoll_client.cpp
	g++ -Wall -std=c++0x -g -o epoll_client epoll_client.cpp	

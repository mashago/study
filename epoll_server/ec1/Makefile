all: platform epoll_client 

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf epoll_client	

epoll_client: epoll_client.cpp
	g++ -Wall -g -std=c++0x -o epoll_client epoll_client.cpp	

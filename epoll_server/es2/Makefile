all: platform epoll_server_mt

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf epoll_server_mt

epoll_server_mt: epoll_server_mt.cpp
	g++ -Wall -g -std=c++0x -o epoll_server_mt epoll_server_mt.cpp	-lrt -lpthread

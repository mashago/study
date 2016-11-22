default: platform HttpServer 

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf HttpServer

HttpServer: HttpServer.cpp
	g++ -Wall -g --std=c++0x -L/usr/local/lib -o HttpServer HttpServer.cpp -levent -pthread

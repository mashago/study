default: platform HttpServer HttpClient

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf HttpServer
	rm -rf HttpClient

HttpServer: HttpServer.cpp
	g++ -Wall -g --std=c++0x -o HttpServer HttpServer.cpp

HttpClient: HttpClient.cpp
	g++ -Wall -g --std=c++0x -o HttpClient HttpClient.cpp

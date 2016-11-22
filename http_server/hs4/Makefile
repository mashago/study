all: platform http_server http_test

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf http_server
	rm -rf http_test
	
http_server: http_server.cpp http_util.h http_connect.h http_request.h http_response.h
	g++ -Wall -std=c++0x -g -o http_server http_server.cpp -pthread
	
http_test: http_test.cpp http_util.h
	g++ -Wall -std=c++0x -g -o http_test http_test.cpp	

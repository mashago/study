default: platform HttpServer 

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf HttpSocketManager.o
	rm -rf HttpServer.o
	rm -rf HttpServer

HttpSocketManager.o: HttpSocketManager.cpp HttpSocketManager.h
	g++ --std=c++11 -c HttpSocketManager.cpp

HttpServer.o: HttpServer.cpp HttpSocketManager.h
	g++ --std=c++11 -c HttpServer.cpp

HttpServer: HttpServer.o HttpSocketManager.o
	g++ --std=c++11 -o HttpServer HttpSocketManager.o HttpServer.o

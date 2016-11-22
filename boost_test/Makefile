all: platform asio_test asio_server_block asio_server_async test

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf asio_test
	rm -rf asio_server_block
	rm -rf asio_server_async
	rm -rf test

LIB:=boost_system boost_thread-mt
	
asio_test: asio_test.cpp
	g++ -Wall -std=c++0x -g -o $@ $< $(addprefix -l, $(LIB))
	
asio_server_block: asio_server_block.cpp
	g++ -Wall -std=c++0x -g -o $@ $< $(addprefix -l, $(LIB))
	
asio_server_async: asio_server_async.cpp
	g++ -Wall -std=c++0x -g -o $@ $< $(addprefix -l, $(LIB))

test: test.cpp
	g++ -Wall -std=c++0x -g -o $@ $< $(addprefix -l, $(LIB))
	

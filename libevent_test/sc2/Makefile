default: platform server_test 

OS:=$(shell uname -s)

CXXFLAG:=-g -Wall -std=c++0x
LIB:=-levent

platform:
	@echo Platform: $(OS)

clean:
	rm -rf server_test
	rm -rf *.dSYM

server_test: server_test.cpp 
	g++ $(CXXFLAG) -o $@ $< $(LIB)


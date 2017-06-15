default: platform connect_test

OS:=$(shell uname -s)

CXXFLAG:=-g -Wall -std=c++11
LIB:=-levent

platform:
	@echo Platform: $(OS)

clean:
	rm -rf connect_test
	rm -rf *.dSYM

connect_test: connect_test.cpp 
	g++ $(CXXFLAG) -o $@ $< $(LIB)

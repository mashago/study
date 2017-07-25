default: platform timer_test

OS:=$(shell uname -s)

CXXFLAG:=-g -Wall -std=c++0x
LIB:=-levent

platform:
	@echo Platform: $(OS)

clean:
	rm -rf timer_test
	rm -rf *.dSYM

timer_test: timer_test.cpp 
	g++ $(CXXFLAG) -o $@ $< $(LIB)

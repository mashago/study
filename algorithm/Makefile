default:	platform test

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf test

test:	test.cpp
	g++ -Wall -std=c++0x -o test test.cpp 


default: platform Test

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf Test

Test: Test.cpp ClassFactory.h
	g++ -Wall -std=c++11 -o Test Test.cpp
	

default: platform callgrind_test

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf callgrind_test
	rm -rf *.dSYM

callgrind_test: callgrind_test.cpp
	g++ -g -o callgrind_test callgrind_test.cpp

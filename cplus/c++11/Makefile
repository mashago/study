default:platform rvalue_reference lambda wrapper TaskThreadTest

OS:=$(shell uname -s)

CFLAG:=-Wall -g -std=c++11

platform:
	@echo Platform: $(OS)

clean:
	rm -rf rvalue_reference lambda wrapper TaskThreadTest
	rm -rf *.dSYM

rvalue_reference:	rvalue_reference.cpp 
	g++ $(CFLAG) -O0 -o $@ $^

lambda:	lambda.cpp 
	g++ $(CFLAG) -o $@ $^

wrapper:	wrapper.cpp 
	g++ $(CFLAG) -o $@ $^

TaskThreadTest:	TaskThreadTest.cpp 
	g++ $(CFLAG) -o $@ $^ -pthread


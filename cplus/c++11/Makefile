default:platform rvalue lambda wrapper TaskThreadTest

OS:=$(shell uname -s)

CFLAG:=-Wall -g -std=c++11

platform:
	@echo Platform: $(OS)

clean:
	rm -rf rvalue lambda wrapper TaskThreadTest
	rm -rf *.dSYM

rvalue:	rvalue.cpp 
	g++ $(CFLAG) -O0 -o $@ $^

lambda:	lambda.cpp 
	g++ $(CFLAG) -o $@ $^

wrapper:	wrapper.cpp 
	g++ $(CFLAG) -o $@ $^

TaskThreadTest:	TaskThreadTest.cpp 
	g++ $(CFLAG) -o $@ $^ -pthread


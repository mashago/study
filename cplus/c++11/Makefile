default:platform rvalue lambda wrapper TaskThreadTest forward_test raw_string_literal

OS:=$(shell uname -s)

CFLAG:=-Wall -g -std=c++11

platform:
	@echo Platform: $(OS)

clean:
	rm -rf rvalue lambda wrapper TaskThreadTest forward_test raw_string_literal
	rm -rf *.dSYM

rvalue:	rvalue.cpp 
# g++ $(CFLAG) -O0 -o $@ $^
	g++ $(CFLAG) -o $@ $^

lambda:	lambda.cpp 
	g++ $(CFLAG) -o $@ $^

wrapper:	wrapper.cpp 
	g++ $(CFLAG) -o $@ $^

TaskThreadTest:	TaskThreadTest.cpp 
	g++ $(CFLAG) -o $@ $^ -pthread

forward_test:	forward_test.cpp 
	g++ $(CFLAG) -o $@ $^
 
raw_string_literal:	raw_string_literal.cpp 
	g++ $(CFLAG) -o $@ $^

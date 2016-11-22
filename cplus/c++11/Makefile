default: platform rvalue lambda wrapper 

OS:=$(shell uname -s)

CFLAG:=-Wall -g -std=c++11

platform:
	@echo Platform: $(OS)

clean:
	rm -rf rvalue
	rm -rf lambda
	rm -rf wrapper
	rm -rf *.dSYM

rvalue:	rvalue_reference.cpp 
	g++ $(CFLAG) -o rvalue rvalue_reference.cpp

lambda:	lambda.cpp 
	g++ $(CFLAG) -o lambda lambda.cpp

wrapper:	wrapper.cpp 
	g++ $(CFLAG) -o wrapper wrapper.cpp


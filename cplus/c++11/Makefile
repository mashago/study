default:platform rvalue lambda wrapper TaskThreadTest forward_test raw_string_literal bind_test random_engine enumclass_test

OS:=$(shell uname -s)

CXX:=g++
CPP_FLAGS:=-Wall -g -std=c++11

platform:
	@echo Platform: $(OS)

clean:
	rm -rf rvalue lambda wrapper TaskThreadTest forward_test raw_string_literal bind_test random_engine enumclass_test
	rm -rf *.dSYM

rvalue:	rvalue.cpp 
	# $(CXX) $(CPP_FLAGS) -O0 -o $@ $^
	$(CXX) $(CPP_FLAGS) -o $@ $^

lambda:	lambda.cpp 
	$(CXX) $(CPP_FLAGS) -o $@ $^

wrapper:	wrapper.cpp 
	$(CXX) $(CPP_FLAGS) -o $@ $^

TaskThreadTest:	TaskThreadTest.cpp 
	$(CXX) $(CPP_FLAGS) -o $@ $^ -pthread

forward_test:	forward_test.cpp 
	$(CXX) $(CPP_FLAGS) -o $@ $^
 
raw_string_literal:	raw_string_literal.cpp 
	$(CXX) $(CPP_FLAGS) -o $@ $^
 
bind_test:	bind_test.cpp 
	$(CXX) $(CPP_FLAGS) -o $@ $^
 
random_engine:	random_engine.cpp 
	$(CXX) $(CPP_FLAGS) -o $@ $^
 
enumclass_test:	enumclass_test.cpp 
	$(CXX) $(CPP_FLAGS) -o $@ $^

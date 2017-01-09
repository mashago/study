default: platform test_typename friend_tp function_tp member_tp variadic_tp

OS:=$(shell uname -s)

CXX:=g++
CPP_FLAGS:=-Wall -std=c++11

platform:
	@echo Platform: $(OS)

clean:
	rm -rf test_typename friend_tp function_tp member_tp variadic_tp
	rm -rf *.dSYM

test_typename: test_typename.cpp
	$(CXX) $(CPP_FLAGS) -o $@ $<

friend_tp:	friend_tp.cpp 
	$(CXX) $(CPP_FLAGS) -o $@ $<

function_tp:	function_tp.cpp 
	$(CXX) $(CPP_FLAGS) -o $@ $<

member_tp:	member_tp.cpp 
	$(CXX) $(CPP_FLAGS) -o $@ $<

variadic_tp:	variadic_tp.cpp 
	$(CXX) $(CPP_FLAGS) -o $@ $<


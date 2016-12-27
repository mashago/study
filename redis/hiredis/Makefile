default: platform test_string test_hash test_list

CXX:=g++
CPPFLAG:=-Wall
LIB:=-lhiredis

OS:=$(shell uname -sm)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf test_string
	rm -rf test_hash
	rm -rf test_list

test_string: test_string.cpp
	$(CXX) $(CPPFLAG) -o $@ $^ $(LIB)

test_hash: test_hash.cpp
	$(CXX) $(CPPFLAG) -o $@ $^ $(LIB)

test_list: test_list.cpp
	$(CXX) $(CPPFLAG) -o $@ $^ $(LIB)


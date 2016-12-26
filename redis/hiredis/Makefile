default: platform test_string test_hash

CXX:=g++
CPPFLAG:=-Wall
LIB:=-lhiredis

OS:=$(shell uname -sm)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf test_string
	rm -rf test_hash

test_string: test_string.cpp
	$(CXX) $(CPPFLAG) -o $@ $^ $(LIB)

test_hash: test_hash.cpp
	$(CXX) $(CPPFLAG) -o $@ $^ $(LIB)


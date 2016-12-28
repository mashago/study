default: platform test_string test_hash test_list test_set test_zset test_lua

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
	rm -rf test_set
	rm -rf test_zset
	rm -rf test_lua

test_string: test_string.cpp
	$(CXX) $(CPPFLAG) -o $@ $^ $(LIB)

test_hash: test_hash.cpp
	$(CXX) $(CPPFLAG) -o $@ $^ $(LIB)

test_list: test_list.cpp
	$(CXX) $(CPPFLAG) -o $@ $^ $(LIB)

test_set: test_set.cpp
	$(CXX) $(CPPFLAG) -o $@ $^ $(LIB)

test_zset: test_zset.cpp
	$(CXX) $(CPPFLAG) -o $@ $^ $(LIB)

test_lua: test_lua.cpp
	$(CXX) $(CPPFLAG) -o $@ $^ $(LIB)

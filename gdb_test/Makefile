default: platform test_gdb

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf test_gdb
	rm -rf test_gdb.dSYM

test_gdb: test_gdb.cpp
	g++ -g -o test_gdb test_gdb.cpp

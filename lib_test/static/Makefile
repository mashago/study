default: platform my_static_lib.o libmy_static.a test

OS:=$(shell uname -s)

CFLAG:=-Wall -g
LIB_PATH:=.
LIB:=my_static

platform:
	@echo Platform: $(OS)

clean:
	rm -rf my_static_lib.o
	rm -rf libmy_static.a
	rm -rf test
	rm -rf *.dSYM

my_static_lib.o: my_static_lib.cpp 
	g++ -c -o $@ $^

libmy_static.a: my_static_lib.o
	ar -crus $@ $^

test: test.cpp 
	g++ $(CFLAG) -o $@ $^ $(addprefix -L, $(LIB_PATH)) $(addprefix -l, $(LIB))

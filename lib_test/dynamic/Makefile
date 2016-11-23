default: platform my_dynamic_lib.o libmy_dynamic.so test my_dynamic_lib2.o libmy_dynamic2.so test2

OS:=$(shell uname -s)

CFLAG:=-Wall -g
LIB_PATH:=.
LIB:=my_dynamic

platform:
	@echo Platform: $(OS)

clean:
	rm -rf my_dynamic_lib.o
	rm -rf libmy_dynamic.so
	rm -rf my_dynamic_lib2.o
	rm -rf libmy_dynamic2.so
	rm -rf test
	rm -rf test2
	rm -rf *.dSYM

my_dynamic_lib.o: my_dynamic_lib.cpp 
	g++ -fPIC -c -o $@ $^

libmy_dynamic.so: my_dynamic_lib.o
	g++ -shared -fPIC -o $@ $^

test: test.cpp 
	g++ $(CFLAG) -o $@ $^ $(addprefix -L, $(LIB_PATH)) $(addprefix -l, $(LIB))


my_dynamic_lib2.o: my_dynamic_lib2.cpp 
	g++ -fPIC -c -o $@ $^

libmy_dynamic2.so: my_dynamic_lib2.o
	g++ -shared -fPIC -o $@ $^

# -rdynamic is require when usr dlopen()
test2: test2.cpp 
	g++ $(CFLAG) -rdynamic -o $@ $^ -ldl


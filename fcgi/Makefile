all: platform demo1 demo2

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf demo1
	rm -rf demo2
	
demo1: demo1.cpp 
	g++ -Wall -std=c++0x -g -o demo1 demo1.cpp -lfcgi
	
demo2: demo2.cpp 
	g++ -Wall -std=c++0x -g -o demo2 demo2.cpp -lfcgi

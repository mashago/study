default: platform test

OS:=$(shell uname -s)


platform:
	@echo Platform: $(OS)

clean:
	rm -rf test

test: test.cpp 
	g++ -Wall -o test test.cpp -lcurl

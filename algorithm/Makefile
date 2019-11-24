default:	platform test offer

OS:=$(shell uname -s)
CFLAG:=-Wall -std=c++11

platform:
	@echo Platform: $(OS)

clean:
	rm -rf test
	rm -rf offer

test:	test.cpp astar.cpp
	g++ $(CFLAG) -o $@ $^

offer:	offer.cpp
	g++ $(CFLAG) -o $@ $^

default: platform test

CFLAG:=-Wall -std=c++0x -g

INC:=
LIB_PATH:=
LIB:=

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf test

test: test.cpp
	g++ $(CFLAG) -o $@ $(addprefix -I, $(INC)) $< $(addprefix -L, $(LIB_PATH)) $(addprifix -l, $(LIB))

all: platform test1

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

# $@ : target file
# $^ : all depended file
# $< : first depended file

test1: test1.sh
	sh -n $^

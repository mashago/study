
default:	platform main

CFLAG:= -Wall -std=c99

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean: 
	rm -rf main

main:	main.c tool.c
	gcc -Wall -o $@ $^


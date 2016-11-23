
default:	platform process_test


OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean: 
	rm -rf process_test

process_test:	process_test.c
	gcc -Wall -o process_test process_test.c

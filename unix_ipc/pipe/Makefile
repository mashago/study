default: platform pipe_test fifo_test 

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf pipe_test
	rm -rf fifo_test

pipe_test: pipe_test.c
	gcc -o pipe_test pipe_test.c

fifo_test: fifo_test.c
	gcc -o fifo_test fifo_test.c

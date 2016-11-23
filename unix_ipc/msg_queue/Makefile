default: platform mq_sv mq_server mq_client mq_test

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf mq_sv
	rm -rf mq_server
	rm -rf mq_client
	rm -rf mq_test

mq_sv: mq_sv.c
	gcc -o mq_sv mq_sv.c

mq_server: mq_server.c
	gcc -o mq_server mq_server.c

mq_client: mq_client.c
	gcc -o mq_client mq_client.c

mq_test: mq_test.c
	gcc -Wall -o mq_test mq_test.c -lpthread 

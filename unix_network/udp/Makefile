default: platform udp_server udp_client

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf udp_server
	rm -rf udp_client

udp_server: udp_server.c
	gcc -o udp_server udp_server.c

udp_client: udp_client.c
	gcc -o udp_client udp_client.c

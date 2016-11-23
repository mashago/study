#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
	printf("hello world\n");

	const char * hostname = "1057cd7.nat123.net";
	// const char * hostname = "www.baidu.com";
	// const char * hostname = "t3.17kapai.com";
	// const char * hostname = "114.215.133.1";
	
	printf("hostname=%s\n", hostname);

	/*
	 * struct hostent {
	 * 	char 	*h_name;
	 * 	char 	**h_aliases;
	 * 	int 	h_addrtype; // AF_INET
	 * 	int 	h_length; // 4, sizeof(struct in_addr);
	 * 	char 	**h_addr_list; // ptr to array to ipv4 address numeric
	 * 	}
	 */
	struct hostent *hptr;
	char **pptr;
	char ip[50];

	// struct hostent *gethostbyname(const char *hostname);
	hptr= gethostbyname(hostname);
	if (hptr == NULL) {
		printf("gethostbyname:empty\n");
		return -1;
	}

	printf("h_name=%s\n", hptr->h_name);

	for (pptr=hptr->h_aliases; *pptr!=NULL; pptr++) {
		printf("h_aliases=%s\n", *pptr);
	}

	printf("h_addrtype=%d\n", hptr->h_addrtype);

	printf("h_length=%d\n", hptr->h_length);

	int i = 0;
	for (pptr=hptr->h_addr_list; *pptr!=NULL; pptr++)
	{
		// printf("h_addr_list=%s\n", *pptr); // *pptr is numeric
		inet_ntop(hptr->h_addrtype, *pptr, ip, sizeof(ip));
		printf("h_addr_list[%d]=%s\n", i, ip);
		i++;
	}

	// *pptr is first address in h_addr_list, is ready a numeric, just memcpy to sockaddr_in.sin_addr
	// memcpy(&sin->sin_addr, *pptr, sizeof(struct in_addr));


	return 0;
}

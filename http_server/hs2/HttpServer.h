#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

/*
 * init listen socket, return a listen socket fd
 */
int init_listen(int port);

/*
 * recv client socket, and send back to client, close fd in the end
 */
int handle_client(int fd);

/*
 * for test, recv client socket, and send back to client, close fd in the end
 */
int handle_client_test(int fd);


/*
 * handle get request
 */
void do_get(int fd, const char *url);

/*
 * handle post request
 */
void do_post(int fd, const char *url);

/*
 * send a success http header to client
 */
void send_success_header(int fd);

/*
 * send a 404 not found page to client
 */
void send_not_found(int fd);

/*
 * send a 501 unimplemented page to client
 */
void send_unimplemented(int fd);

/*
 * send a simple web page to client
 */
void send_simple_page(int fd);

/*
 * send web page to client
 */
void send_page(int fd, const char *filename);

/*
 * send a file to client
 */
void send_file(int fd, FILE *file);

#endif

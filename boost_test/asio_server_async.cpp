extern "C"
{
#include <stdio.h>
}
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

#define SERVER_PORT 7710

class MyServer {
public:
	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
	MyServer(boost::asio::io_service &service, boost::asio::ip::tcp::endpoint &ep) : acc_(service, ep) {
	};

	~MyServer() {
	};

	void start_accept()
	{
		// 1.create socket
		// 2.aync_accept
		
		socket_ptr psock(new boost::asio::ip::tcp::socket(acc_.get_io_service()));

		// http://www.boost.org/doc/libs/1_60_0/doc/html/boost_asio/reference/basic_socket_acceptor/async_accept/overload2.html
		// handler
		// The handler to be called when the accept operation completes. Copies will be made of the handler as required. The function signature of the handler must be:
		// void handler(
		//   const boost::system::error_code& error // Result of operation.
		// );
		acc_.async_accept(*psock, boost::bind(&MyServer::handle_accept, this, psock, _1));
	}

	void handle_accept(socket_ptr psock, const boost::system::error_code &err)
	{
		// 1.check error
		// 2.listen accept
		// 3.handle client
		
		if (err) return;
		start_accept();

		// TODO handle client

	}

private:
	boost::asio::ip::tcp::acceptor acc_;
};

int main(int argc, char **argv)
{
	printf("hello asio_server_async\n");
	printf("SERVER_PORT=%d\n", SERVER_PORT);

	// 1.define io_service
	// 2.init endpoint
	// 3.init acceptor
	// 4.start accept
	// 5.io_service run

	boost::asio::io_service service;
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), SERVER_PORT);

	MyServer my_server(service, ep);
	my_server.start_accept();

	service.run();

	return 0;
}

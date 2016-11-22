extern "C"
{
#include <stdio.h>
}
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

void client_session(socket_ptr psock)
{
	printf("client_session\n");
	while (true)
	{
		char data[512];

		// XXX read_some will throw when EOF
		// size_t len = psock->read_some(boost::asio::buffer(data));

		boost::system::error_code ec;
		size_t len = psock->read_some(boost::asio::buffer(data), ec);
		printf("len=%lu\n", len);

		if (len == 0)
		{
			// read_some error
			// printf("read_some error\n");
			std::cout << "read_some error " << ec << std::endl;

        	if(ec) return;
		}

		if (len > 0)
		{
			data[len] = '\0';
			printf("data=[%s]\n", data);
			boost::asio::write(*psock, boost::asio::buffer("ok\n", 3));
		}
	}
}

int main(int argc, char **argv)
{
	printf("hello asio\n");


	// 1.define io_service
	// 2.init endpoint
	// 3.init acceptor
	// 4.loop accept client
	// 4.1 create client socket
	// 4.2 blocking accept
	// 4.3 create new thread to handle client

	boost::asio::io_service service;
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), 7710);
	boost::asio::ip::tcp::acceptor acc(service, ep);

	while (true)
	{
		socket_ptr psock(new boost::asio::ip::tcp::socket(service));
		// block
		acc.accept(*psock);
		boost::thread(boost::bind(client_session, psock));
	}

	return 0;
}

#include <iostream>
#include <string>
#include <memory>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "tcp_server.h"

int main()
{
	try
	{
		boost::asio::io_service io_service;
		TCP_Server server(io_service);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
    return 0;
}


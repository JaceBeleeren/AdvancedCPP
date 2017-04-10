#pragma once
#include <boost/asio.hpp>
#include "tcp_connection.h"
class TCP_Server
{
public:
	TCP_Server(boost::asio::io_service& io_service);
	~TCP_Server();
private:
	void TCP_Server::start_accept();
	void handle_accept(TCP_Connection::pointer new_connection, const boost::system::error_code& error);
	
	boost::asio::ip::tcp::acceptor acceptor_;
};


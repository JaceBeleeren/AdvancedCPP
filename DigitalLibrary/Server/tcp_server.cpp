#include "tcp_server.h"



TCP_Server::TCP_Server(boost::asio::io_service& io_service) : acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), Protocol::PORT))
{
	start_accept();
}


TCP_Server::~TCP_Server()
{
}

void TCP_Server::start_accept()
{
	TCP_Connection::pointer new_connection = TCP_Connection::create(acceptor_.get_io_service());

	acceptor_.async_accept
	(
		new_connection->socket(),
		boost::bind(&TCP_Server::handle_accept, this, new_connection, boost::asio::placeholders::error)
	);
}

void TCP_Server::handle_accept(TCP_Connection::pointer new_connection, const boost::system::error_code& error)
{
	if (!error)
	{
		new_connection->start();
		start_accept();
	}
}


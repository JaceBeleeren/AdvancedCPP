#pragma once
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "../Common/protocol.h"
#include "../Common/Actions/actionlogin.h"
#include "../Common/Actions/actionaddbook.h"

class TCP_Connection : public boost::enable_shared_from_this<TCP_Connection>
{
public:
	typedef boost::shared_ptr<TCP_Connection> pointer;
	static pointer create(boost::asio::io_service& io_service);
	boost::asio::ip::tcp::socket& getSocket();
	void start();
	unsigned int id; //id of connected user 0 = not logged in

private:
	TCP_Connection(boost::asio::io_service& io_service);
	void handle_write(const boost::system::error_code& error, std::size_t n, unsigned char action);
	void handle_read_header(const boost::system::error_code& error, std::size_t n, std::shared_ptr<char> header);
	void handle_read_payload(const boost::system::error_code& error, std::size_t n, std::shared_ptr<char> payload, unsigned int payload_size, unsigned char action, unsigned char following);
	void disconnect();

	boost::asio::ip::tcp::socket socket;
	unsigned int errorcount;
};


#pragma once
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "../Common/protocol.h"

class TCP_Connection : public boost::enable_shared_from_this<TCP_Connection>
{
public:
	typedef boost::shared_ptr<TCP_Connection> pointer;
	static pointer create(boost::asio::io_service& io_service);
	boost::asio::ip::tcp::socket& socket();
	void start();

private:
	TCP_Connection(boost::asio::io_service& io_service);
	void handle_write(const boost::system::error_code& error, std::size_t n );
	void handle_read_header(const boost::system::error_code& error, std::size_t n);
	void handle_read_payload(const boost::system::error_code& error, std::size_t n);
	void disconnect();

	boost::asio::ip::tcp::socket socket_;
	unsigned int errorcount;

	char header[Protocol::HEADER_SIZE];
	char payload[Protocol::MAX_PAYLOAD_SIZE + 1]; //+1 for additional \0
	char data[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1];
	unsigned int payload_size;
	unsigned char action;
	unsigned char following;
};


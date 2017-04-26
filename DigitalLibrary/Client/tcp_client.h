#pragma once

#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "../Common/protocol.h"
#include "../Common/Actions/actionlogin.h"

typedef boost::asio::ip::tcp tcp;

class TCP_Client : public boost::enable_shared_from_this<TCP_Client>
{
public:
	TCP_Client(std::string host, boost::asio::io_service& io_service/*, tcp::resolver::iterator endpoint_iterator*/);
	~TCP_Client();
	void start();
	void start_async();
	void start_sync();

private:
	void handle_connect(const boost::system::error_code& error);
	void handle_write(const boost::system::error_code& error, std::size_t n);
	void handle_read_header(const boost::system::error_code& error, std::size_t n);
	void handle_read_payload(const boost::system::error_code& error, std::size_t n);
	void disconnect();
	void reconnect();

	boost::asio::ip::tcp::socket socket;
	boost::asio::ip::tcp::acceptor acceptor;

	unsigned int errorcount;
	unsigned int reconnect_count;

	char header[Protocol::HEADER_SIZE];
	char payload[Protocol::MAX_PAYLOAD_SIZE + 1]; //+1 for additional \0
	char data[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1];

	unsigned int payload_size;
	unsigned char action;
	unsigned char following;
};
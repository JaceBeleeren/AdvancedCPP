#pragma once

#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "../Common/protocol.h"
#include "../Common/Actions/actionlogin.h"
#include "../Common/Actions/actionaddbook.h"

typedef boost::asio::ip::tcp tcp;

class TCP_Client : public boost::enable_shared_from_this<TCP_Client>
{
public:
	TCP_Client(std::string host, boost::asio::io_service& io_service/*, tcp::resolver::iterator endpoint_iterator*/);
	~TCP_Client();
	void start();
	void start_async();

private:
	void handle_connect(const boost::system::error_code& error);
	void handle_write(const boost::system::error_code& error, std::size_t n, unsigned char action);
	void handle_read_header(const boost::system::error_code& error, std::size_t n, std::shared_ptr<char> header);
	void handle_read_payload(const boost::system::error_code& error, std::size_t n, std::shared_ptr<char> payload, unsigned int payload_size, unsigned char action, unsigned char following);
	void disconnect();
	void reconnect();

	boost::asio::ip::tcp::socket socket;
	boost::asio::ip::tcp::acceptor acceptor;

	unsigned int errorcount;
	unsigned int reconnect_count;
};
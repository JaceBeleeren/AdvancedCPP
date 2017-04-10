#include "tcp_connection.h"



TCP_Connection::TCP_Connection(boost::asio::io_service& io_service) : socket_(io_service), errorcount(0)
{
}

boost::shared_ptr<TCP_Connection> TCP_Connection::create(boost::asio::io_service& io_service)
{
	return pointer(new TCP_Connection(io_service));
}

boost::asio::ip::tcp::socket& TCP_Connection::socket()
{
	return socket_;
}

void TCP_Connection::start()
{
	std::cout << std::endl << "Start" << std::endl;
	boost::asio::async_read
	(
		socket_,
		boost::asio::buffer(header, Protocol::HEADER_SIZE),
		boost::bind(&TCP_Connection::handle_read_header, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
	);
}

void TCP_Connection::handle_write(const boost::system::error_code& error, std::size_t n)
{
	if (!error)
	{
		errorcount = 0;
		std::cout << "handle write" << std::endl;
		start(); //continue reading
	}
	else
	{
		errorcount++;
		if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error) || errorcount > Protocol::MAX_ERRORCOUNT)//client disconnect
		{
			disconnect();
		}
		else
		{
			std::cout << "Error handle_write:" << error;
			start(); //continue reading
		}
	}
}

void TCP_Connection::handle_read_header(const boost::system::error_code& error, std::size_t n)
{
	std::cout << "Read Header:" << std::endl;
	if (!error && Protocol::decode_header(&payload_size, &action, &following, header))
	{
		errorcount = 0;
		boost::asio::async_read
		(
			socket_,
			boost::asio::buffer(payload, payload_size),
			boost::bind(&TCP_Connection::handle_read_payload, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
		);
		std::cout << "Payloadsize: " << payload_size << std::endl;
	}
	else
	{
		errorcount++;
		if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error) || errorcount > Protocol::MAX_ERRORCOUNT)//client disconnect
		{
			disconnect();
		}
		else
		{
			std::cout << "Error handle_read_header::" << error << std::endl;
			std::cout << "Payloadsize: " << payload_size << std::endl;
			start();
		}
	}

}

void TCP_Connection::handle_read_payload(const boost::system::error_code& error, std::size_t n)
{
	std::cout << "Read payload:" << std::endl;
	if (!error)
	{
		errorcount = 0;
		payload[payload_size] = 0; //end string if it is not already
		std::cout << "Payload: " << payload << std::endl;

		/*do some stuff with payload and generate response*/
		int action = 2;
		int following = 0;
		std::string response = "you are damn cool";
		payload_size = response.size();
		strncpy(data + Protocol::HEADER_SIZE, response.c_str(), payload_size);
		data[Protocol::HEADER_SIZE + payload_size] = '\0';

		if (0)//some communication may not need a response
		{
			start();
		}
		else
		{
			/*answer with data*/
			Protocol::encode_header(payload_size, action, following, data);
			boost::asio::async_write
			(
				socket_,
				boost::asio::buffer(data, Protocol::HEADER_SIZE + payload_size),
				boost::bind(&TCP_Connection::handle_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
			);
		}
	}
	else
	{
		errorcount++;
		if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error) || errorcount > Protocol::MAX_ERRORCOUNT)//client disconnect
		{
			disconnect();
		}
		else
		{
			std::cout << "Error handle_read_payload:" << error << std::endl;
			start();
		}
	}
}

void TCP_Connection::disconnect()
{
	std::cout << "Disconnect" << std::endl;
}
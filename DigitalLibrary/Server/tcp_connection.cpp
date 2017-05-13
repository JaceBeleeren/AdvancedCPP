#include "tcp_connection.h"



TCP_Connection::TCP_Connection(boost::asio::io_service& io_service) : socket(io_service), errorcount(0), id(0)
{
}

boost::shared_ptr<TCP_Connection> TCP_Connection::create(boost::asio::io_service& io_service)
{
	return pointer(new TCP_Connection(io_service));
}

boost::asio::ip::tcp::socket& TCP_Connection::getSocket()
{
	return socket;
}

void TCP_Connection::start()
{
	std::cout << std::endl << "Start" << std::endl;
	std::shared_ptr<char> header = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE], Protocol::array_deleter<char>());
	boost::asio::async_read
	(
		socket,
		boost::asio::buffer(header.get(), Protocol::HEADER_SIZE),
		boost::bind(&TCP_Connection::handle_read_header, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, header)
	);
}

void TCP_Connection::handle_write(const boost::system::error_code& error, std::size_t n, unsigned char action)
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

void TCP_Connection::handle_read_header(const boost::system::error_code& error, std::size_t n, std::shared_ptr<char> header)
{
	std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());
	unsigned int payload_size;
	unsigned char action;
	unsigned char following;
	std::cout << "Read Header:" << std::endl;
	if (!error && Protocol::decode_header(&payload_size, &action, &following, header))
	{
		errorcount = 0;
		boost::asio::async_read
		(
			socket,
			boost::asio::buffer(payload.get(), payload_size),
			boost::bind(&TCP_Connection::handle_read_payload, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, payload, payload_size, action, following)
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

void TCP_Connection::handle_read_payload(const boost::system::error_code& error, std::size_t n, std::shared_ptr<char> payload, unsigned int payload_size, unsigned char action, unsigned char following)
{
	std::cout << "Read payload: " << action << std::endl;
	if (!error)
	{
		errorcount = 0;

		std::shared_ptr<char> response_payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
		std::shared_ptr<char> response_action_payload;//payload excluding header
		unsigned char response_action = 0;
		unsigned char response_following = 0;
		unsigned int response_payload_size = 0;


		if (action == Protocol::ACTION_LOGIN)
		{
			ActionLogin login;
			login.parseToStruct(payload);
			unsigned int id;
			//process login
			this->id = id;

			//response
			//create response
		}

		else if (action == Protocol::ACTION_ADD_BOOK)
		{
			ActionAddBook addBook;
			addBook.parseToStruct(payload);
			std::cout << "Add Book" << std::endl;
			std::cout << "Title: " << addBook.payload_struct.book.get()->title << std::endl;
			std::cout << "Author: " << addBook.payload_struct.book.get()->author << std::endl;
			std::cout << "Summary: " << addBook.payload_struct.book.get()->summary << std::endl;
			std::cout << "Year: " << addBook.payload_struct.book.get()->year << std::endl;
			std::cout << "ISBN: " << addBook.payload_struct.book.get()->getIsbn() << std::endl;
			std::cout << "Amount: " << addBook.payload_struct.book.get()->getAmount() << std::endl;

			//response
			addBook.response_struct.id = 666;
			addBook.response_struct.response = "Book with title " + addBook.payload_struct.book.get()->title + "received";
			addBook.response_parseToPayload();

			response_action_payload = addBook.response_payload;
			response_payload_size = addBook.response_size;
			response_action = addBook.action_response;
			response_following = 0;
		}

		else
		{
			std::cout << std::endl << "Received action " << std::to_string(action) << " is not a valid action!" << std::endl;
			start();
			return;
		}

		Protocol::encode_header(response_payload_size, response_action, response_following, response_payload);
		memcpy(response_payload.get() + Protocol::HEADER_SIZE, response_action_payload.get(), response_payload_size);
		std::cout << "Response Size:" << response_payload_size << std::endl;
		response_payload.get()[Protocol::HEADER_SIZE + payload_size] = '\0';

		if (0)//some communication may not need a response
		{
			start();
		}
		else
		{
			/*answer with data*/
			boost::asio::async_write
			(
				socket,
				boost::asio::buffer(response_payload.get(), Protocol::HEADER_SIZE + response_payload_size),
				boost::bind(&TCP_Connection::handle_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, response_action)
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
	id = 0;
	boost::system::error_code ec_shutdown;
	socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec_shutdown);
	if (ec_shutdown)
	{
		std::cout << "An error occured shutting down socket: " << ec_shutdown << std::endl;
	}
	boost::system::error_code ec_close;
	socket.close(ec_close);
	if (ec_close)
	{
		std::cout << "An error occured closing socket: " << ec_close << std::endl;
	}
	std::cout << "Disconnect" << std::endl;
}
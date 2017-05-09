
#include "tcp_client.h"

TCP_Client::TCP_Client(std::string host, boost::asio::io_service& io_service)
	: socket(io_service), acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), Protocol::PORT)), errorcount(0), reconnect_count(0)
{
	tcp::resolver resolver(socket.get_io_service());
	tcp::resolver::query query(host, Protocol::SERVICE);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

	//sync
	/*
	boost::asio::connect(socket, endpoint_iterator);
	start_sync();
	//*/

	//async
	///*
	boost::asio::async_connect(socket, endpoint_iterator, boost::bind(&TCP_Client::handle_connect, this, boost::asio::placeholders::error));
	//*/

	//start();
	
}


TCP_Client::~TCP_Client()
{
}

void TCP_Client::start()
{
	//start_async();
	//start_sync();
}


void TCP_Client::handle_connect(const boost::system::error_code& error)
{
	if (!error)
	{
		start_async();
	}
	else
	{
		std::cout << "Error handle connect:" << error << std::endl;
	}
}

void TCP_Client::start_async()
{
	std::cout << std::endl << "Press <Enter> to add a cool book and receive response" << std::endl;
	getchar();
	unsigned char choosen_action = 0;

	std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
	std::shared_ptr<char> action_payload;//payload excluding header
	unsigned char action;
	unsigned char following;
	unsigned int payload_size;

	if (choosen_action == 0)
	{
		ActionAddBook addBook;
		addBook.payload_struct.title = "How to C++";
		addBook.payload_struct.author = "Ich";
		addBook.payload_struct.summary = "Isn't a title enough?";
		addBook.payload_struct.publisher = "FH-Kiel";
		addBook.payload_struct.year = 2017;
		addBook.payload_struct.isbn = "666-666-666";
		addBook.payload_struct.amount = 10;
		addBook.parseToPayload();
		action_payload = addBook.payload;
		payload_size = addBook.payload_size;
		action = Protocol::ACTION_ADD_BOOK;
		following = 0;
	}
		
	Protocol::encode_header(payload_size, action, following, payload);
	memcpy(payload.get() + Protocol::HEADER_SIZE, action_payload.get(), payload_size);
	payload.get()[Protocol::HEADER_SIZE + payload_size] = '\0';
	std::cout << "Input Size:" << payload_size << std::endl;

	boost::asio::async_write
	(
		socket,
		boost::asio::buffer(payload.get(), Protocol::HEADER_SIZE + payload_size),
		boost::bind(&TCP_Client::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, action)
	);
}

void TCP_Client::handle_write(const boost::system::error_code& error, std::size_t n, unsigned char action)
{
	if (!error)
	{
		if (0/*action == some action*/) //somtimes no need to wait for response
		{
			start_async();
		}
		else
		{
			std::shared_ptr<char> header = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE], Protocol::array_deleter<char>());
			boost::asio::async_read
			(
				socket,
				boost::asio::buffer(header.get(), Protocol::HEADER_SIZE),
				boost::bind(&TCP_Client::handle_read_header, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, header)
			);
		}
	}
	else
	{
		errorcount++;
		if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error) || errorcount > Protocol::MAX_ERRORCOUNT)//client reconnect
		{
			reconnect();
		}
		else
		{
			std::cout << "Error handle write:" << error << std::endl;
			start_async();
		}
	}
}

void TCP_Client::handle_read_header(const boost::system::error_code& error, std::size_t n, std::shared_ptr<char> header)
{
	std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());
	unsigned int payload_size;
	unsigned char action;
	unsigned char following;
	if (!error && Protocol::decode_header(&payload_size, &action, &following, header))
	{
		boost::asio::async_read
		(
			socket,
			boost::asio::buffer(payload.get(), payload_size),
			boost::bind(&TCP_Client::handle_read_payload, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, payload, payload_size, action, following)
		);
	}
	else
	{
		errorcount++;
		if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error) || errorcount > Protocol::MAX_ERRORCOUNT)//client reconnect
		{
			reconnect();
		}
		else
		{
			std::cout << "Error handle read header:" << error << std::endl;
			start_async();
		}
	}

}

void TCP_Client::handle_read_payload(const boost::system::error_code& error, std::size_t n, std::shared_ptr<char> payload, unsigned int payload_size, unsigned char action, unsigned char following)
{
	if (!error)
	{
		std::cout << "Handle read payload with action " << (int)action << " and " << payload_size << " bytes" << std::endl;
		payload.get()[payload_size] = 0; //end string if it is not already
		if (action == Protocol::ACTION_ADD_BOOK_RESPONSE)
		{
			ActionAddBook addBookResponse;
			addBookResponse.response_parseToStruct(payload);
			std::cout << "Add Book Response" << std::endl;
			std::cout << "ID: " << addBookResponse.response_struct.id << std::endl;
			std::cout << "Response: " << addBookResponse.response_struct.response << std::endl;
		}
		start_async();
	}
	else
	{
		errorcount++;
		if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error) || errorcount > Protocol::MAX_ERRORCOUNT)//client reconnect
		{
			reconnect();
		}
		else
		{
			std::cout << "Error handle read payload:" << error << std::endl;
			start_async();
		}
	}
}
//*/


void TCP_Client::disconnect()
{
	std::cout << "Disconnect" << std::endl;
}
void TCP_Client::reconnect()
{
	if (reconnect_count > Protocol::MAX_RECONNECTCOUNT)
		disconnect();
	else
	{
		reconnect_count++;
		std::cout << "Reconnect: " << reconnect_count << ". try" << std::endl;
		if (1)
		{
			std::cout << "Reconnected" << std::endl;
		}
		else
			reconnect();
	}
}
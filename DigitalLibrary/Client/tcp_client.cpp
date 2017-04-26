
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


void TCP_Client::start_sync()
{
		while(1)
		{
			boost::array<char, 128> buf;
			boost::system::error_code error;


			std::cout << "Input Message:\n";
			std::string input;
			std::cin >> input;

			char data[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1];

			//fill header
			int size = input.size();
			Protocol::encode_header(size, 1, 0, data);

			//fill payload with input
			strncpy(data + Protocol::HEADER_SIZE, input.c_str(), size);
			data[Protocol::HEADER_SIZE + size] = '\0';

			socket.send(boost::asio::buffer(data, input.length() + Protocol::HEADER_SIZE));
			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.
			std::cout << "Output Message:" << std::endl;
			size_t len = socket.read_some(boost::asio::buffer(buf), error);
			std::cout << "Length: " << len << std::endl;
			std::cout.write(buf.data(), len);
			std::cout << std::endl;
		}
}

void TCP_Client::start_async()
{
	std::cout << std::endl << "Press <Enter> to submit data and receive response" << std::endl;
	getchar();
	ActionLogin payload;
	payload.payload_struct.user = "Erwin aus dem Wald";
	payload.payload_struct.test = 12345;
	payload.payload_struct.string = "abcdefg";
	payload.parseToPayload();
	char data[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1];
	Protocol::encode_header(payload.payload_size, Protocol::ACTION_LOGIN, 0, data);
	memcpy(data + Protocol::HEADER_SIZE, payload.payload.get(), payload.payload_size);
	std::cout << "Input Size:" << payload.payload_size << std::endl;

	boost::asio::async_write
	(
		socket,
		boost::asio::buffer(data, Protocol::HEADER_SIZE + payload.payload_size),
		boost::bind(&TCP_Client::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
	);
}

void TCP_Client::handle_write(const boost::system::error_code& error, std::size_t n)
{
	if (!error)
	{
		if (0) //somtimes no need to wait for response
		{
			start_async();
		}
		else
		{
			boost::asio::async_read
			(
				socket,
				boost::asio::buffer(header, Protocol::HEADER_SIZE),
				boost::bind(&TCP_Client::handle_read_header, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
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

void TCP_Client::handle_read_header(const boost::system::error_code& error, std::size_t n)
{
	if (!error && Protocol::decode_header(&payload_size, &action, &following, header))
	{
		boost::asio::async_read
		(
			socket,
			boost::asio::buffer(payload, payload_size),
			boost::bind(&TCP_Client::handle_read_payload, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
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

void TCP_Client::handle_read_payload(const boost::system::error_code& error, std::size_t n)
{
	if (!error)
	{
		std::cout << "Handle read payload" << std::endl;
		payload[payload_size] = 0; //end string if it is not already
		std::cout << "Payload: " << payload << std::endl;
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
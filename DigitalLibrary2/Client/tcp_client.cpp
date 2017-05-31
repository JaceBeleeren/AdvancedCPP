
#include "tcp_client.h"
#include "..\Common\InputFct.h"
#include "..\Common\Actions\actioncreateuser.h"
#include "..\Common\Actions\actionmodifyuser.h"
#include "..\Common\Actions\actiondeleteuser.h"

#include "..\Common\user.h"

TCP_Client::TCP_Client(std::string newHost, boost::asio::io_service& io_service)
	: socket(io_service), errorcount(0), reconnect_count(0), host(newHost)
{
	connect();
}


TCP_Client::~TCP_Client()
{
}


void TCP_Client::handle_connect(const boost::system::error_code& error)
{
	if (!error && socket.is_open())
	{
		reconnect_count = 0;
		std::cout << "Connected" << std::endl;
		start_async();
	}
	else
	{
		std::cout << "Error handle connect:" << error << std::endl;
		reconnect();
	}
}

void TCP_Client::start_async()
{
	char input;
	std::tuple<char, std::shared_ptr<user>> check ; // 0 = user choice what to do   1= pointer to user object
	bool pw_check;
	InputFct in;
	std::shared_ptr<user> loggedin = NULL;
	
	
		std::cout << "\n Welcome to the digital library system.\n What do you like to do ?" << std::endl;
		std::cout << "Press 'l' for login \n or press c for create a new account" << std::endl;
		std::cin >> input;

		user u(1, "max", "1234");
		


		switch (input)
		{
		case 'l':

			loggedin = login(in.login());
			
			{
			check = in.userScreen(loggedin);

				if(std::get<0>(check) == 'm')      //modifyUser
				{ 
					 modifyUser(std::get<1>(check));
			    }
				if (std::get<0>(check) == 'c')		//changePassword
				{
					modifyUser(std::get<1>(check));
				}
				if (std::get<0>(check) == 'd')		//delete Account
				{
					deleteUser(std::get<1>(check));
				}

				

		    }
			/*else
			{
				return;
			}
*/
		   break;
		
			// Next case

		case 'c':
			createUser(in.createUser());
			break;


		default:
			std::cout << "Invalid Input" << std::endl;
			break; 
		}

		
		start_async(); 
	
	//Book b =  in.addBook();

	//

	//std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
	//std::shared_ptr<char> action_payload;//payload excluding header
	//unsigned char action;
	//unsigned char following;
	//unsigned int payload_size;

	//
	//
	//	std::shared_ptr<Book> book = std::shared_ptr<Book>(new Book(b.title, b.author, b.summary, b.publisher, b.year, b.getIsbn(), b.getAmount()));
	//	ActionAddBook addBook;
	//	addBook.payload_struct.book = book;
	//	addBook.parseToPayload();
	//	action_payload = addBook.payload;
	//	payload_size = addBook.payload_size;
	//	action = addBook.action;
	//	following = 0;
	//
	//	
	//Protocol::encode_header(payload_size, action, following, payload);
	//memcpy(payload.get() + Protocol::HEADER_SIZE, action_payload.get(), payload_size);
	//payload.get()[Protocol::HEADER_SIZE + payload_size] = '\0';
	//std::cout << "Input Size:" << payload_size << std::endl;

	//boost::asio::async_write
	//(
	//	socket,
	//	boost::asio::buffer(payload.get(), Protocol::HEADER_SIZE + payload_size),
	//	boost::bind(&TCP_Client::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, action)
	//);

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
		else
		{
			std::cout << std::endl << "Received action " << std::to_string(action) << " is not a valid action!" << std::endl;
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
	boost::system::error_code ec_shutdown;
	socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec_shutdown);
	if (ec_shutdown && ec_shutdown != boost::system::errc::not_connected /*socket not connected*/)
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
void TCP_Client::reconnect()
{
	if (reconnect_count > Protocol::MAX_RECONNECTCOUNT)
	{
		char answer;
		do
		{

			std::cout << "Tried reconnecting for " << Protocol::MAX_RECONNECTCOUNT << "times. Try again? (Y/N)" << std::endl;
			std::cin >> answer;
			if (answer == 'Y' || answer == 'y')
			{
				reconnect_count = 1;
				std::cout << "Reconnect: " << reconnect_count << ". try" << std::endl;
				connect(true);
			}
			else if(answer == 'N' || answer == 'n')
			{
				disconnect();
			}
		} while (answer != 'N' && answer != 'n' &&answer != 'Y' &&answer != 'y');
		
	}
	else
	{
		reconnect_count++;
		std::cout << "Reconnect: " << reconnect_count << ". try" << std::endl;
		connect(true);
	}
}

void TCP_Client::connect(bool disconnect_indicator)
{
	if(disconnect_indicator)
		disconnect();
	tcp::resolver resolver(socket.get_io_service());
	tcp::resolver::query query(host, Protocol::SERVICE);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	boost::asio::async_connect(socket, endpoint_iterator, boost::bind(&TCP_Client::handle_connect, this, boost::asio::placeholders::error));
}

void TCP_Client::createUser(user u)
{
	
	std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
	std::shared_ptr<char> action_payload;//payload excluding header
	unsigned char action;
	unsigned char following;
	unsigned int payload_size;
		
		std::shared_ptr<user> user1 = std::shared_ptr<user>(new user(u.getuserType(), u.username, u.getPassword()));		
		ActionCreateUser createUser;
		createUser.payload_struct.user = user1;
		createUser.parseToPayload();
		action_payload = createUser.payload;
		payload_size = createUser.payload_size;
		action = createUser.action;
		following = 0;
}

std::shared_ptr<user> TCP_Client::login(user u)
{
	std::shared_ptr<user> log = NULL; 
	user t;
	bool check; 

	log = u.getUser(u.username); 
	check = u.getPassword().compare(u.getPassword());

	if (check == true)
	{
		return log;
	}
	else
	{
		std::cout << "Login failed. Try again." << std::endl; 
		return NULL; 
	}
	
}

void TCP_Client::modifyUser(std::shared_ptr<user> u)
{
	std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
	std::shared_ptr<char> action_payload;//payload excluding header
	unsigned char action;
	unsigned char following;
	unsigned int payload_size;

	 
	ActionModifyUser modifyUser;
	modifyUser.payload_struct.user = u;
	modifyUser.parseToPayload();
	action_payload = modifyUser.payload;
	payload_size = modifyUser.payload_size;
	action = modifyUser.action;
	following = 0;


}

void TCP_Client::deleteUser(std::shared_ptr<user> u)
{
	std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
	std::shared_ptr<char> action_payload;//payload excluding header
	unsigned char action;
	unsigned char following;
	unsigned int payload_size;

	ActionDeleteUser deleteUser;
	deleteUser.payload_struct.user = u;
	deleteUser.parseToPayload();
	action_payload = deleteUser.payload;
	payload_size = deleteUser.payload_size;
	action = deleteUser.action;
	following = 0;


}
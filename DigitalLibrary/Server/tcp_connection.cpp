#include "tcp_connection.h"
#include "../Common/Actions/actioncreateuser.h"
#include "../Common/Actions/actionmodifyuser.h"
#include "../Common/Actions/actiondeleteuser.h"
#include "..\Common\Actions\actionaddbook.h"
#include "..\Common\Actions\actiondeletebook.h"
#include "..\Common\Actions\actionlogin.h"
#include "..\Common\Actions\actionlogout.h"
#include "..\Common\Actions\actionmodifybook.h"
#include "..\Common\Actions\actionshowbooks.h"
#include "..\Common\Actions\actionshowborrowedbooks.h"
#include "..\Common\Actions\actionshowusers.h"
#include "..\Common\Actions\actionborrowbook.h"


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
			std::cout << "BookID:" << addBook.payload_struct.book.get()->getId() << std::endl;
			std::cout << "Title: " << addBook.payload_struct.book.get()->title << std::endl;
			std::cout << "Author: " << addBook.payload_struct.book.get()->author << std::endl;
			std::cout << "Summary: " << addBook.payload_struct.book.get()->summary << std::endl;
			std::cout << "Year: " << addBook.payload_struct.book.get()->year << std::endl;
			std::cout << "ISBN: " << addBook.payload_struct.book.get()->getIsbn() << std::endl;
			std::cout << "Amount: " << addBook.payload_struct.book.get()->getAmount() << std::endl;

			//response
			addBook.response_struct.id = addBook.payload_struct.book.get()->getId();
			addBook.response_struct.response = "Book with title " + addBook.payload_struct.book.get()->title + "received";
			addBook.response_parseToPayload();

			response_action_payload = addBook.response_payload;
			response_payload_size = addBook.response_size;
			response_action = addBook.action_response;
			response_following = 0;
		}

		else if (action == Protocol::ACTION_CREATE_USER)
		{
			ActionCreateUser createUser; 
			createUser.parseToStruct(payload);
			std::cout << "User created" <<std::endl;

			createUser.response_struct.id = 222; 
			createUser.response_struct.response = "User with the username '"+ createUser.payload_struct.user.get()->username +"' created";
			createUser.response_parseToPayload();
			response_action_payload = createUser.response_payload;
			response_payload_size = createUser.response_size;
			response_action = createUser.action_response;
			response_following = 0;
		}

		else if (action == Protocol::ACTION_MODIFY_USER)
		{
			ActionModifyUser modifyUser;

			modifyUser.parseToStruct(payload);
			std::cout << "Following changes are received:" << std::endl;
			std::cout << "Username:" << modifyUser.payload_struct.user.get()->username <<  std::endl;
			std::cout << "FirstName:" << modifyUser.payload_struct.user.get()->fName << std::endl;
			std::cout << "LastName:" << modifyUser.payload_struct.user.get()->lName << std::endl;
			std::cout << "Mobile Phone:" << modifyUser.payload_struct.user.get()->mPhone << std::endl;
			std::cout << "City:" << modifyUser.payload_struct.user.get()->city << std::endl; 

			modifyUser.response_struct.success = true; 
			modifyUser.response_struct.response ="Changes have been submitted!";
			modifyUser.response_parseToPayload();
			response_action_payload = modifyUser.response_payload;
			response_payload_size = modifyUser.response_size;
			response_action = modifyUser.action_response;
			response_following = 0;

		}

		else if (action == Protocol::ACTION_DELETE_USER)
		{
			ActionDeleteUser deleteUser; 
			deleteUser.parseToStruct(payload); 
		//	std::string name = deleteUser.payload_struct.user.get()->username;
			 

			deleteUser.payload_struct.user.get()->deleteUser(deleteUser.payload_struct.user.get()->username);
			std::cout << "User deleted" << std::endl;			
			
			deleteUser.response_struct.success = true; 
			deleteUser.response_struct.response = /*name +*/ "has been deleted";
			response_action_payload = deleteUser.response_payload;
			response_payload_size = deleteUser.response_size;
			response_action = deleteUser.action_response;
			response_following = 0;
		}

		else if (action == Protocol::ACTION_BORROW_BOOK)
		{
			ActionBorrowBook borrowBook;
			borrowBook.parseToStruct(payload);
			std::cout << "Borrowed Book:" << std::endl;

			Book::borrow(borrowBook.payload_struct.id, borrowBook.payload_struct.u);
			
			borrowBook.response_struct.success = true;
			borrowBook.response_struct.response = "Book has been borrowed by " + borrowBook.payload_struct.username;
			response_action_payload = borrowBook.response_payload;
			response_payload_size = borrowBook.response_size;
			response_action = borrowBook.action_response;
			response_following = 0;
		}
		else if (action == Protocol::ACTION_DELETE_BOOK)
		{
			ActionDeleteBook deleteBook;
			deleteBook.parseToStruct(payload);
			std::cout << "Delete Book with ID" << deleteBook.payload_struct.id << std::endl;



			deleteBook.response_struct.success = true;
			deleteBook.response_struct.response = "Book has been deleted";
			response_action_payload = deleteBook.response_payload;
			response_payload_size = deleteBook.response_size;
			response_action = deleteBook.action_response;
			response_following = 0;
		}
		else if (action == Protocol::ACTION_LOGIN)
		{

		}
		else if (action == Protocol::ACTION_LOGOUT)
		{
			ActionLogout logout; 
			logout.parseToStruct(payload);
			std::cout << "Logout" << std::endl;

			logout.response_struct.success = true;
			response_action_payload = logout.response_payload;
			response_payload_size = logout.response_size;
			response_action = logout.action_response;
			response_following = 0;
		}
		else if (action == Protocol::ACTION_MODIFY_BOOK)
		{
			ActionModifyBook modBook;
			modBook.parseToStruct(payload);
			std::cout << "Modify Book" << std::endl;

			/*std::cout << "Title: " << modBook.payload_struct.book.get()->title << std::endl;
			std::cout << "Author: " << modBook.payload_struct.book.get()->author << std::endl;
			std::cout << "Summary: " << modBook.payload_struct.book.get()->summary << std::endl;
			std::cout << "Year: " << modBook.payload_struct.book.get()->year << std::endl;
			std::cout << "ISBN: " << modBook.payload_struct.book.get()->getIsbn() << std::endl;
			std::cout << "Amount: " << modBook.payload_struct.book.get()->getAmount() << std::endl;*/

			modBook.response_struct.success = true;
			response_action_payload = modBook.response_payload;
			response_payload_size = modBook.response_size;
			response_action = modBook.action_response;
			response_following = 0;
		}
		else if (action == Protocol::ACTION_SHOW_BOOKS)
		{
			ActionShowBooks showB;
			showB.parseToStruct(payload);
			std::cout << "Show Books" << std::endl;

			showB.response_struct;
			showB.response_parseToPayload();
			response_action_payload = showB.response_payload;
			response_payload_size = showB.response_size;
			response_action = showB.action_response;
			response_following = 0;
		}
		else if (action == Protocol::ACTION_SHOW_BORROWED_BOOKS)
		{
			ActionShowBorrowedBooks showBB;
			showBB.parseToStruct(payload);
			std::cout << "Show borrowed Books" << std::endl;

			showBB.response_struct;
			response_action_payload = showBB.response_payload;
			response_payload_size = showBB.response_size;
			response_action = showBB.action_response;
			response_following = 0;

		}
		else if (action == Protocol::ACTION_SHOW_USERS)
		{
			ActionShowUsers showUser;
			showUser.parseToStruct(payload);
			std::cout << "Show Users" << std::endl;

			showUser.response_struct;
			showUser.response_parseToPayload();
			response_action_payload = showUser.response_payload;
			response_payload_size = showUser.response_size;
			response_action = showUser.action_response;
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
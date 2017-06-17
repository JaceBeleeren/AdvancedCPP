#include <iostream>
#include <boost/asio.hpp>
#include "tcp_client.h"
#include "../Common/protocol.h"
#include "../Common/User.h"

typedef boost::asio::ip::tcp tcp;

int main(int argc, char* argv[])
{
	char host[16] = "127.0.0.1";
	try
	{
		int id1;
		
			std::cout << "No serveradress specified." << std::endl;
			std::cout << "Usage:" << std::endl;
			std::cout << "digitalLibraryClient.exe <host>" << std::endl;
			std::cout << "127.0.0.1 is used as standard" << std::endl << std::endl;

			std::shared_ptr<Book> book1 = std::shared_ptr<Book>(new Book());
			book1.get()->constructorBook("testtitle", "testauthor", "testsummary", "testpublisher", 2017, "testisbn", 10);
			id1 = book1.get()->getId();

			std::shared_ptr<User> user1 = std::shared_ptr<User>(new User());
			user1.get()->constructorUser(1,"Max","1234");						// Normal lib user
			std::cout << user1.get()->username; 

			std::shared_ptr<User> user2 = std::shared_ptr<User>(new User());
			user2.get()->constructorUser(2, "Maxine", "1234");						// Employee lib user
			std::cout << user1.get()->username;


		
	/*	else
		{
			strncpy(host, argv[1], 16);
		}*/

		boost::asio::io_service io_service;
		TCP_Client client(host, io_service, id1);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::cout << "Finish" << std::endl;
	std::string a;
	std::cin >> a;
  return 0;
}


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
		int id1 =1;
		
			std::cout << "No serveradress specified." << std::endl;
			std::cout << "Usage:" << std::endl;
			std::cout << "digitalLibraryClient.exe <host>" << std::endl;
			std::cout << "127.0.0.1 is used as standard" << std::endl << std::endl;

			std::shared_ptr<Book> book1 = std::shared_ptr<Book>(new Book());
			book1.get()->constructorBook("testtitle", "testauthor", "testsummary", "testpublisher", 2017, "666", 10);
			//std::cout << "ID	:" << book1.get()->getId() << "		Title:	" << book1.get()->title << std::endl;

			std::shared_ptr<Book> book2 = std::shared_ptr<Book>(new Book());
			book2.get()->constructorBook("Java", "Mustermann", "Java", "Spring", 2014, "0123456789", 5);
			//std::cout << "ID	:" << book2.get()->getId() << "		Title:	" << book2.get()->title << std::endl;

			std::shared_ptr<Book> book3 = std::shared_ptr<Book>(new Book());
			book3.get()->constructorBook("Operating System", "Kinsey", "Windows", "Pearson", 2001, "012345555", 3);
			//std::cout << "ID	:" << book3.get()->getId() << "		Title:	" << book3.get()->title << std::endl;

			std::shared_ptr<Book> book4 = std::shared_ptr<Book>(new Book());
			book4.get()->constructorBook("C++", "Jim", "Programming", "Spring", 2010, "012345554", 7);
			//std::cout << "ID	:" << book4.get()->getId() << "		Title:	" << book4.get()->title << std::endl;

			std::shared_ptr<Book> book5 = std::shared_ptr<Book>(new Book());
			book5.get()->constructorBook("User Expirience", "Ronald", "User Design", "Spring", 2012, "012345566", 9);
			//std::cout << "ID	:" << book5.get()->getId() << "		Title:	" << book5.get()->title << std::endl;

			std::shared_ptr<Book> book6 = std::shared_ptr<Book>(new Book());
			book6.get()->constructorBook("Business Management", "Eckhardt", "Business", "Spring", 2012, "022445566", 4);
			//std::cout << "ID	:" << book6.get()->getId() << "		Title:	" << book6.get()->title << std::endl;

			std::shared_ptr<Book> book7 = std::shared_ptr<Book>(new Book());
			book7.get()->constructorBook("Social Media", "Zuckerberg", "Facebook", "FBPress", 2015, "023445599", 1);
			//std::cout << "ID	:" << book7.get()->getId() << "		Title:	" << book7.get()->title << std::endl;

			std::shared_ptr<Book> book8 = std::shared_ptr<Book>(new Book());
			book8.get()->constructorBook("Math for Beginners", "Mueller", "Basics", "Springer", 2011, "666666669", 5);
			//std::cout << "ID	:" << book8.get()->getId() << "		Title:	" << book8.get()->title << std::endl;

			std::shared_ptr<Book> book9 = std::shared_ptr<Book>(new Book());
			book9.get()->constructorBook("How to pass the math exam", "Mueller", "Pro", "Springer", 2014, "666666666", 15);
			//std::cout << "ID	:" << book9.get()->getId() << "		Title:	" << book9.get()->title << std::endl;

			std::shared_ptr<Book> book10 = std::shared_ptr<Book>(new Book());
			book10.get()->constructorBook("C#", "Mueller", "Programming", "Microsoft", 2016, "1234567889", 5);
			//std::cout << "ID	:" << book10.get()->getId() << "		Title:	" << book10.get()->title << std::endl;


			std::shared_ptr<User> user1 = std::shared_ptr<User>(new User());
			user1.get()->constructorUser(1,"Max","1234");						// Normal lib user
		//	std::cout << user1.get()->username; 

			std::shared_ptr<User> user2 = std::shared_ptr<User>(new User());
			user2.get()->constructorUser(2, "Maxine", "1234");						// Employee lib user
		//	std::cout << user1.get()->username;


		
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


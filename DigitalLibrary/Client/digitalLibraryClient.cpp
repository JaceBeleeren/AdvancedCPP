#include <iostream>
#include <boost/asio.hpp>
#include "tcp_client.h"
#include "../Common/protocol.h"
typedef boost::asio::ip::tcp tcp;
int main(int argc, char* argv[])
{
	char host[16] = "127.0.0.1";
	try
	{
		int id1;
		if (argc != 2)
		{
			std::cout << "No serveradress specified." << std::endl;
			std::cout << "Usage:" << std::endl;
			std::cout << "digitalLibraryClient.exe <host>" << std::endl;
			std::cout << "127.0.0.1 is used as standard" << std::endl << std::endl;

			std::shared_ptr<Book> book1 = std::shared_ptr<Book>(new Book());
			book1.get()->constructorBook("testtitle", "testauthor", "testsummary", "testpublisher", 2017, "testisbn", 10);
			id1 = book1.get()->getId();
		}
		else
		{
			strncpy(host, argv[1], 16);
		}

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
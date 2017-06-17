
#include "tcp_client.h"
#include "..\Common\InputFct.h"
#include "..\Common\user.h"
#include "..\Common\protocol.h"
#include "..\Common\Actions\actioncreateuser.h"
#include "..\Common\Actions\actionmodifyuser.h"
#include "..\Common\Actions\actiondeleteuser.h"
#include "..\Common\Actions\actionmodifyuser.h"
#include "..\Common\Actions\actionshowbooks.h"
#include "..\Common\\Actions\actionshowusers.h"
#include "..\Common\Actions\actionshowborrowedbooks.h"
#include "..\Common\Actions\actionborrowbook.h"
#include "..\Common\Actions\actionmodifybook.h"
#include "..\Common\Actions\actionlogin.h"
#include "..\Common\Actions\\actionlogout.h"
#include "..\Common\Actions\actiondeletebook.h"




TCP_Client::TCP_Client(std::string newHost, boost::asio::io_service& io_service, int id)
	: socket(io_service), errorcount(0), reconnect_count(0), host(newHost)
{
	std::shared_ptr<Book> book1 = std::shared_ptr<Book>(new Book());
	book1.get()->constructorBook("testtitle", "testauthor", "testsummary", "testpublisher", 2017, "testisbn2", 10);
	int id1 = book1.get()->getId();
	std::cout << "Amount of different books: " << Book::books.size() << " Test created test book: " << Book::books.find(id)->second.get()->title << ":" << Book::books.find(id)->second.get()->getIsbn() << " ID: " << id << std::endl;
	connect(id1);
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
	std::tuple<char, std::shared_ptr<User>> check; // 0 = user choice what to do   1= pointer to user object	
	char test = NULL;
	bool pw_check;
	InputFct in;
	std::shared_ptr<User> loggedin = NULL;
	std::shared_ptr<User> u;
	unsigned char choosen_action = 0;


	std::cout << "\n Welcome to the digital library system.\n What do you like to do ?" << std::endl;
	std::cout << "Press 'l' for login \n or press c for create a new account" << std::endl;
	std::cin >> input;

	if (input == 'l')
	{
		loggedin = login(in.login());

		if (loggedin->userType == 1)
		{
			choosen_action = in.userScreen(loggedin);

			std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
			std::shared_ptr<char> action_payload;//payload excluding header
			unsigned char action;
			unsigned char following;
			unsigned int payload_size;


			if (choosen_action == 'm')   // modify user
			{


				std::string fname;
				std::string lname;
				int nr;
				std::string city;

				std::cout << "Enter the data to change:\n" << std::endl;
				std::cout << "Username: You are not allowed to change your username." << std::endl;
				std::cout << "\n First Name:" << std::endl;
				std::cin >> fname;
				std::cout << "\n Last Name:" << std::endl;
				std::cin >> lname;
				std::cout << "\n Mobile Phone:" << std::endl;
				std::cin >> nr;
				std::cout << "\n City\n" << std::endl;
				std::cin >> city;

				loggedin.get()->fName = fname;
				loggedin.get()->lName = lname;
				loggedin.get()->mPhone = nr;
				loggedin.get()->city = city;

				ActionModifyUser modifyUser;
				modifyUser.payload_struct.user = loggedin;
				modifyUser.parseToPayload();
				action_payload = modifyUser.payload;
				payload_size = modifyUser.payload_size;
				action = modifyUser.action;
				following = 0;
			}

			if (choosen_action == 'd')  // delete user
			{
				char check;
				std::cout << "Are you sure?\n Enter y or n" << std::endl;
				std::cin >> check;

				switch (check)
				case 'y':
				{
					if (/*u.get()->lendBooks.empty() ==*/ true)
					{


						//u->deleteUser(u->username);	
						ActionDeleteUser deleteUser;
						deleteUser.payload_struct.user = loggedin;
						deleteUser.parseToPayload();
						action_payload = deleteUser.payload;
						payload_size = deleteUser.payload_size;
						action = deleteUser.action;
						following = 0;

					}
					else
					{
						std::map<unsigned int, std::string>::iterator it;
						std::cout << "You cannot delete account when you still have borrowed books" << std::endl;

						for (it = u.get()->lendBooks.begin(); it != u.get()->lendBooks.end(); it++)
						{
							std::cout << "BookID	:" << it->first << "	 " << "Book Title	:" << it->second << std::endl;
							std::cout << "" << std::endl;
							std::cout << "=========================================================" << std::endl;
						}
						start_async();
					}


				case 'n':
				{
					std::cout << "Nothing happens" << std::endl;
					in.userScreen(loggedin);
				}

				}
			}

			if (choosen_action == 's')  // search book
			{
				std::string title;
				std::string author;
				std::string publisher;
				int year;
				Book b;
				std::list<std::shared_ptr<Book>> search;
				std::list<std::shared_ptr<Book>>::iterator it;
				std::shared_ptr<Book > book;
				int input;

				std::cout << "Enter your search terms:\n" <<
					"(for 'not specified' enter a '0'\n)" << std::endl;
				std::cout << "\nTitle:" << std::endl;
				std::cin >> title;
				std::cout << "\nAuthor:" << std::endl;
				std::cin >> author;
				std::cout << "\Publisher:" << std::endl;
				std::cin >> publisher;
				std::cout << "\Year:" << std::endl;
				std::cin >> year;

				if (title != "0" && author == "0" && publisher == "0" && year == 0)
				{
					search = b.getBooks(title);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;

					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}



				}
				if (title != "0" && author != "0" && publisher == "0" && year == 0)
				{
					search = b.getBooks(title, author);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;
					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}

				}
				if (title != "0" && author != "0" && publisher != "0" && year == 0)
				{
					search = b.getBooks(title, author, publisher);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;
					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}

				}
				if (title != "0" && author != "0" && publisher != "0" && year != 0)
				{
					search = b.getBooks(title, author, publisher, year);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;
					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}

				}
				if (title == "0" && author != "0" && publisher == "0" && year == 0)
				{
					search = b.getBooksbyAuthor(author);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;
					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}

				}
				if (title == "0" && author != "0" && publisher != "0" && year == 0)
				{
					search = b.getBooksbyPublandAuth(author, publisher);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;
					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}

				}
				if (title == "0" && author == "0" && publisher != "0" && year == 0)
				{
					search = b.getBooksbyPubl(publisher);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;
					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}
				}
			}

			if (choosen_action == 'i')  // search by isbn
			{
				std::shared_ptr<Book> b;
				std::string isbn;
				int input;

				std::cout << "Please enter the ISBN number:" << std::endl;
				std::cin >> isbn;


				b = b.get()->getBookbyISBN(isbn);

				std::cout << "" << std::endl;
				std::cout << "========================================================" << std::endl;
				std::cout << "Book ID:" << b->getId() << std::endl;
				std::cout << "Title: " << b->title << std::endl;
				std::cout << "Author: " << b->author << std::endl;
				std::cout << "Summary: " << b->summary << std::endl;
				std::cout << "Publisher: " << b->publisher << std::endl;
				std::cout << "Year: " << b->year << std::endl;
				std::cout << "ISBN: " << b->getIsbn() << std::endl;
				std::cout << "========================================================" << std::endl;
				std::cout << "" << std::endl;

				std::cout << "Do you like to borrow a book?" << std::endl;
				std::cout << "Please enter the book ID:" << std::endl;
				std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
				if (input != 0)
				{
					b.get()->borrow(input, loggedin);
				}
			}

			if (choosen_action == 'b')  // show borrowed books
			{
				std::map<unsigned int, std::string>::iterator it;

				std::cout << "You lend the following book(s):" << std::endl;
				std::cout << "=========================================================" << std::endl;

				for (it = u.get()->lendBooks.begin(); it != u.get()->lendBooks.end(); it++)
				{
					std::cout << "BookID	:" << it->first << "	 " << "Book Title	:" << it->second << std::endl;
					std::cout << "" << std::endl;
					std::cout << "=========================================================" << std::endl;
				}
			}

			if (choosen_action == 'e')  // exit / logout
			{
				ActionLogout logout;
				logout.payload_struct;
				logout.parseToPayload();
				action_payload = logout.payload;
				payload_size = logout.payload_size;
				action = logout.action;
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
		if (loggedin->userType == 2)
		{
			choosen_action = in.employeeScreen(loggedin);

			std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
			std::shared_ptr<char> action_payload;//payload excluding header
			unsigned char action;
			unsigned char following;
			unsigned int payload_size;

			if (choosen_action == 'm')
			{
				std::string fname;
				std::string lname;
				int nr;
				std::string city;

				std::cout << "Enter the data to change:\n" << std::endl;
				std::cout << "Username: You are not allowed to change your username." << std::endl;
				std::cout << "\n First Name:" << std::endl;
				std::cin >> fname;
				std::cout << "\n Last Name:" << std::endl;
				std::cin >> lname;
				std::cout << "\n Mobile Phone:" << std::endl;
				std::cin >> nr;
				std::cout << "\n City\n" << std::endl;
				std::cin >> city;

				loggedin.get()->fName = fname;
				loggedin.get()->lName = lname;
				loggedin.get()->mPhone = nr;
				loggedin.get()->city = city;

				ActionModifyUser modifyUser;
				modifyUser.payload_struct.user = loggedin;
				modifyUser.parseToPayload();
				action_payload = modifyUser.payload;
				payload_size = modifyUser.payload_size;
				action = modifyUser.action;
				following = 0;

			}   // Modify Account

			if (choosen_action == 'a')
			{
				std::string title;
				std::string author;
				std::string sum;
				std::string isbn;
				std::string publ;
				int year;
				int amount;

				std::cout << "Enter data for adding a book" << std::endl;
				std::cout << "Title: " << std::endl;
				std::cin >> title;
				std::cout << "Author: " << std::endl;
				std::cin >> author;
				std::cout << "Summary: " << std::endl;
				std::cin >> sum;
				std::cout << "Publisher" << std::endl;
				std::cin >> publ;
				std::cout << "Year: " << std::endl;
				std::cin >> year;
				std::cout << "ISBN: " << std::endl;
				std::cin >> isbn;
				std::cout << "Amount: " << std::endl;
				std::cin >> amount;

				std::shared_ptr<Book> book = std::shared_ptr<Book>(new Book());
				book.get()->constructorBook(title, author, sum, publ, year, isbn, amount);
				ActionAddBook addBook;
				addBook.payload_struct.book = book;
				addBook.parseToPayload();
				action_payload = addBook.payload;
				payload_size = addBook.payload_size;
				action = addBook.action;
				following = 0;

			}  // addBook

			if (choosen_action == 'c')
			{
				std::string title;
				std::string author;
				std::string sum;
				std::string isbn;
				std::string publ;
				int year;
				int amount;
				int bookId;

				std::shared_ptr<Book> b;

				std::cout << "Enter the BookID to find the Book:" << std::endl;
				std::cin >> bookId;
				std::cout << "Enter the new data:" << std::endl;
				std::cout << "Title: " << std::endl;
				std::cin >> title;
				std::cout << "Author: " << std::endl;
				std::cin >> author;
				std::cout << "Summary: " << std::endl;
				std::cin >> sum;
				std::cout << "Publisher" << std::endl;
				std::cin >> publ;
				std::cout << "Year: " << std::endl;
				std::cin >> year;
				std::cout << "ISBN: " << std::endl;
				std::cin >> isbn;
				std::cout << "Amount: " << std::endl;
				std::cin >> amount;

				b.get()->modifyBook(title, author, sum, publ, year, isbn, amount, bookId);

				ActionModifyBook modBook;
				modBook.payload_struct.book /*= b*/;
				modBook.parseToPayload();
				action_payload = modBook.payload;
				payload_size = modBook.payload_size;
				action = modBook.action;
				following = 0;


			}

			if (choosen_action == 'f')
			{
				int bookId;
				std::shared_ptr<Book> b;
				std::cout << "Enter ID for the book to delete:" << std::endl;
				std::cin >> bookId;
				b.get()->deleteBook(bookId);

				ActionDeleteBook delBook;
				delBook.payload_struct.id = bookId;
				delBook.parseToPayload();
				action_payload = delBook.payload;
				payload_size = delBook.payload_size;
				action = delBook.action;
				following = 0;


			}

			if (choosen_action == 's')
			{
				std::string title;
				std::string author;
				std::string publisher;
				int year;
				Book b;
				std::list<std::shared_ptr<Book>> search;
				std::list<std::shared_ptr<Book>>::iterator it;
				std::shared_ptr<Book > book;
				int input;

				std::cout << "Enter your search terms:\n" <<
					"(for 'not specified' enter a '0'\n)" << std::endl;
				std::cout << "\nTitle:" << std::endl;
				std::cin >> title;
				std::cout << "\nAuthor:" << std::endl;
				std::cin >> author;
				std::cout << "\Publisher:" << std::endl;
				std::cin >> publisher;
				std::cout << "\Year:" << std::endl;
				std::cin >> year;

				if (title != "0" && author == "0" && publisher == "0" && year == 0)
				{
					search = b.getBooks(title);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;

					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}



				}
				if (title != "0" && author != "0" && publisher == "0" && year == 0)
				{
					search = b.getBooks(title, author);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;
					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}

				}
				if (title != "0" && author != "0" && publisher != "0" && year == 0)
				{
					search = b.getBooks(title, author, publisher);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;
					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}

				}
				if (title != "0" && author != "0" && publisher != "0" && year != 0)
				{
					search = b.getBooks(title, author, publisher, year);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;
					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}

				}
				if (title == "0" && author != "0" && publisher == "0" && year == 0)
				{
					search = b.getBooksbyAuthor(author);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;
					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}

				}
				if (title == "0" && author != "0" && publisher != "0" && year == 0)
				{
					search = b.getBooksbyPublandAuth(author, publisher);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;
					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}

				}
				if (title == "0" && author == "0" && publisher != "0" && year == 0)
				{
					search = b.getBooksbyPubl(publisher);
					for (it = search.begin(); it != search.end(); it++)
					{
						book = *it;

						std::cout << "" << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "Book ID:" << book->getId() << std::endl;
						std::cout << "Title: " << book->title << std::endl;
						std::cout << "Author: " << book->author << std::endl;
						std::cout << "Summary: " << book->summary << std::endl;
						std::cout << "Publisher: " << book->publisher << std::endl;
						std::cout << "Year: " << book->year << std::endl;
						std::cout << "ISBN: " << book->getIsbn() << std::endl;
						std::cout << "========================================================" << std::endl;
						std::cout << "" << std::endl;
					}
					std::cout << "Do you like to borrow a book?" << std::endl;
					std::cout << "Please enter the book ID:" << std::endl;
					std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
					std::cin >> input;
					if (input != 0)
					{
						b.borrow(input, loggedin);
					}
				}
			}

			if (choosen_action == 'i')
			{
				std::shared_ptr<Book> b;
				std::string isbn;
				int input;

				std::cout << "Please enter the ISBN number:" << std::endl;
				std::cin >> isbn;


				b = b.get()->getBookbyISBN(isbn);

				std::cout << "" << std::endl;
				std::cout << "========================================================" << std::endl;
				std::cout << "Book ID:" << b->getId() << std::endl;
				std::cout << "Title: " << b->title << std::endl;
				std::cout << "Author: " << b->author << std::endl;
				std::cout << "Summary: " << b->summary << std::endl;
				std::cout << "Publisher: " << b->publisher << std::endl;
				std::cout << "Year: " << b->year << std::endl;
				std::cout << "ISBN: " << b->getIsbn() << std::endl;
				std::cout << "========================================================" << std::endl;
				std::cout << "" << std::endl;

				std::cout << "Do you like to borrow a book?" << std::endl;
				std::cout << "Please enter the book ID:" << std::endl;
				std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
				if (input != 0)
				{
					b.get()->borrow(input, loggedin);
				}
			}

			if (choosen_action == 'p')
			{
				std::string username;
				std::cout << "Enter a username to search:" << std::endl;
				std::cin >> username;

				std::map<unsigned int, std::string>::iterator it;
				std::shared_ptr<User> user;
				if (user.get()->users.find(username) != user.get()->users.end())
				{
					std::cout << "Username   :" << user.get()->users.find(username)->second->username << std::endl;
					std::cout << "First Name :" << user.get()->users.find(username)->second->fName << std::endl;
					std::cout << "Last Name  :" << user.get()->users.find(username)->second->lName << std::endl;
					std::cout << "City       :" << user.get()->users.find(username)->second->city << std::endl;
					std::cout << "Mobile Phone  :" << user.get()->users.find(username)->second->mPhone << std::endl;
					std::cout << "Number of borrowed books  :" << user.get()->users.find(username)->second->countBorrowedBooks << std::endl;
					std::cout << "User lend following book(s):" << std::endl;
					std::cout << "" << std::endl;

					for (it = user.get()->lendBooks.begin(); it != user.get()->lendBooks.end(); it++)
					{
						std::cout << "BookID		:" << it->first << "Booktitle	:" << it->second << std::endl;
						std::cout << "" << std::endl;
						std::cout << "===========================================================" << std::endl;
					}
				}

				ActionShowUsers showUser;
				showUser.payload_struct;
				showUser.parseToPayload();
				action_payload = showUser.payload;
				payload_size = showUser.payload_size;
				action = showUser.action;
				following = 0;

			}

			if (choosen_action == 'z')
			{
				Book b;
				std::map<unsigned int, std::shared_ptr<Book>>::iterator it;
				for (it = b.books.begin(); it != b.books.end(); it++)
				{
					std::cout << "============================================================" << std::endl;
					std::cout << "" << std::endl;
					std::cout << "Number    :" << it->first << std::endl;
					std::cout << "ISBN:	    :" << it->second.get()->getIsbn() << std::endl;
					std::cout << "Title	    :" << it->second.get()->title << std::endl;
					std::cout << "Author	:" << it->second.get()->author << std::endl;
					std::cout << "Summary   :" << it->second.get()->summary << std::endl;
					std::cout << "Publisher :" << it->second.get()->publisher << std::endl;
					std::cout << "Year      :" << it->second.get()->year << std::endl;
					std::cout << "Amount    :" << it->second.get()->getAmount() << std::endl;
					std::cout << "" << std::endl;
					std::cout << "============================================================" << std::endl;
					//std::cout << "Avaible:" << it->second.get()->title << std::endl;

					ActionShowBooks showB;
					showB.payload_struct;
					showB.parseToPayload();
					action_payload = showB.payload;
					payload_size = showB.payload_size;
					action = showB.action;
					following = 0;
				}
			}

			if (choosen_action == 'x')
			{
				std::shared_ptr<Book> b;
				std::map<unsigned int, std::shared_ptr<Book>>::iterator it;

				std::cout << "Exemplars of the following book(s) are lend:" << std::endl;
				for (it = b.get()->books.begin(); it != b.get()->books.end(); it++)
				{
					if (it->second->getBorrowed() > 0)
					{
						std::cout << it->second->title << std::endl;
						std::cout << "" << std::endl;
						std::cout << "========================================" << std::endl;
					}
				}

				ActionShowBorrowedBooks showBB;
				showBB.payload_struct;
				showBB.parseToPayload();
				action_payload = showBB.payload;
				payload_size = showBB.payload_size;
				action = showBB.action;
				following = 0;
			}

			if (choosen_action == 'u')
				{
					User u;
					std::map<std::string, std::shared_ptr<User>>::iterator it;
					for (it = u.users.begin(); it != u.users.end(); it++)
					{
						std::cout << "============================================================" << std::endl;
						std::cout << "" << std::endl;
						std::cout << "Username	    :" << it->second.get()->username << std::endl;
						std::cout << "First Name    :" << it->second.get()->fName << std::endl;
						std::cout << "Last Name     :" << it->second.get()->lName << std::endl;
						std::cout << "City          :" << it->second.get()->city << std::endl;
						std::cout << "Mobile Phone  :" << it->second.get()->mPhone << std::endl;
						std::cout << "Usertype      :" << it->second.get()->userType << std::endl;
						std::cout << "Number of borrowes books:" << it->second.get()->countBorrowedBooks << std::endl;
						std::cout << "" << std::endl;
						std::cout << "============================================================" << std::endl;
						//std::cout << "Avaible:" << it->second.get()->title << std::endl;
					}
					ActionShowUsers showUsers;
					showUsers.payload_struct;
					showUsers.parseToPayload();
					action_payload = showUsers.payload;
					payload_size = showUsers.payload_size;
					action = showUsers.action;
					following = 0;
				}

			if (choosen_action == 'd')
				{
					if (/*u.get()->lendBooks.empty() ==*/ true)
					{
						std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
						std::shared_ptr<char> action_payload;//payload excluding header
						unsigned char action;
						unsigned char following;
						unsigned int payload_size;

						//u->deleteUser(u->username);	
						ActionDeleteUser deleteUser;
						deleteUser.payload_struct.user = loggedin;
						deleteUser.parseToPayload();
						action_payload = deleteUser.payload;
						payload_size = deleteUser.payload_size;
						action = deleteUser.action;
						following = 0;


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
					else
					{
						std::map<unsigned int, std::string>::iterator it;
						std::cout << "You cannot delete account when you still have borrowed books" << std::endl;

						for (it = u.get()->lendBooks.begin(); it != u.get()->lendBooks.end(); it++)
						{
							std::cout << "BookID	:" << it->first << "	 " << "Book Title	:" << it->second << std::endl;
							std::cout << "" << std::endl;
							std::cout << "=========================================================" << std::endl;
						}
						start_async();
					}
				}

			if (choosen_action == 'e')
				{
					std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
					std::shared_ptr<char> action_payload;//payload excluding header
					unsigned char action;
					unsigned char following;
					unsigned int payload_size;

					ActionLogout logout;
					logout.payload_struct;
					logout.parseToPayload();
					action_payload = logout.payload;
					payload_size = logout.payload_size;
					action = logout.action;
					following = 0;

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
			

			}
		

		if (input == 'c')
		{
			std::string username;
			std::string pw;

			std::cout << "Please enter the necessary data to create an account:\n" << std::endl;
			std::cout << "Username:" << std::endl;
			std::cin >> username;
			std::cout << "Password:" << std::endl;
			std::cin >> pw;


			std::shared_ptr<User> user1 = std::shared_ptr<User>(new User());
			user1.get()->constructorUser(1, username, pw);

			std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
			std::shared_ptr<char> action_payload;//payload excluding header
			unsigned char action;
			unsigned char following;
			unsigned int payload_size;

			ActionCreateUser createUser;
			createUser.payload_struct.user = user1;
			createUser.parseToPayload();
			action_payload = createUser.payload;
			payload_size = createUser.payload_size;
			action = createUser.action;
			following = 0;

			Protocol::encode_header(payload_size, action, following, payload);
			memcpy(payload.get() + Protocol::HEADER_SIZE, action_payload.get(), payload_size);
			payload.get()[Protocol::HEADER_SIZE + payload_size] = '\0';
			std::cout << "input size:" << payload_size << std::endl;

			boost::asio::async_write
			(
				socket,
				boost::asio::buffer(payload.get(), Protocol::HEADER_SIZE + payload_size),
				boost::bind(&TCP_Client::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, action)
			);
		}

		if (input != 'c' && input != 'l')
		{
			std::cout << "Invalid Input" << std::endl;
			start_async();
		}



	}


	
	void TCP_Client::handle_write(const boost::system::error_code& error, std::size_t n, unsigned char action)
		{
			if (!error)
			{
				if (action == Protocol::ACTION_ADD_BOOK) //somtimes no need to wait for response
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

				else if (action == Protocol::ACTION_DELETE_BOOK_RESPONSE)
				{
					ActionDeleteBook delBookResponse;
					delBookResponse.response_parseToStruct(payload);
					std::cout << "Add Book Response" << std::endl;
					std::cout << "" << delBookResponse.response_struct.success << std::endl;
					std::cout << "Response: " << delBookResponse.response_struct.response << std::endl;
				}
				else if (action == Protocol::ACTION_CREATE_USER_RESPONSE)
				{
					ActionCreateUser createUserResponse;
					createUserResponse.response_parseToStruct(payload);
					std::cout << "Create User Response" << std::endl;
					std::cout << "ID: " << createUserResponse.response_struct.id << std::endl;
					std::cout << "Response: " << createUserResponse.response_struct.response << std::endl;

				}
				else if (action == Protocol::ACTION_DELETE_USER_RESPONSE)
				{
					ActionDeleteUser deleteUserResponse;
					deleteUserResponse.response_parseToStruct(payload);
					std::cout << "Delete User Response" << std::endl;
					std::cout << "ID: " << deleteUserResponse.response_struct.success << std::endl;
					std::cout << "Response: " << deleteUserResponse.response_struct.response << std::endl;
				}
				else if (action == Protocol::ACTION_MODIFY_USER_RESPONSE)
				{
					ActionModifyUser modifyUserResponse;
					modifyUserResponse.response_parseToStruct(payload);
					std::cout << "Modify User Response" << std::endl;
					std::cout << "ID: " << modifyUserResponse.response_struct.success << std::endl;
					std::cout << "Response: " << modifyUserResponse.response_struct.response << std::endl;
				}
				else if (action == Protocol::ACTION_SHOW_USERS_RESPONSE)
				{
					ActionShowUsers showUsersResponse;
					showUsersResponse.response_parseToStruct(payload);
					std::cout << "Show all Users Response" << std::endl;
					/*std::cout << "ID: " << showUsersResponse.response_struct. << std::endl;
					std::cout << "Response: " << showUsersResponse.response_struct. << std::endl;*/
				}

				else if (action == Protocol::ACTION_SHOW_BORROWED_BOOKS_RESPONSE)
				{
					ActionShowBorrowedBooks showborrowedBooksResponse;
					showborrowedBooksResponse.response_parseToStruct(payload);
					std::cout << "Show borrowed books Response" << std::endl;
					/*std::cout << "ID: " << showborrowedBooksResponse.response_struct << std::endl;
					std::cout << "Response: " << showborrowedBooksResponse.response_struct.response << std::endl;*/
				}
				else if (action == Protocol::ACTION_BORROW_BOOK_RESPONSE)
				{
					ActionBorrowBook borrowBookResponse;
					borrowBookResponse.response_parseToStruct(payload);
					std::cout << "Borrow Book Response" << std::endl;
					std::cout << "ID: " << borrowBookResponse.response_struct.success << std::endl;
					std::cout << "Response: " << borrowBookResponse.response_struct.response << std::endl;
				}
				else if (action == Protocol::ACTION_MODIFY_BOOK_RESPONSE)
				{
					ActionModifyBook modifyBookResponse;
					modifyBookResponse.response_parseToStruct(payload);
					std::cout << "Modify Book Response" << std::endl;
					std::cout << "ID: " << modifyBookResponse.response_struct.success << std::endl;
					std::cout << "Response: " << modifyBookResponse.response_struct.response << std::endl;
				}
				else if (action == Protocol::ACTION_LOGIN_RESPONSE)
				{
					ActionLogin loginResponse;
					loginResponse.response_parseToStruct(payload);
					std::cout << "Login Response" << std::endl;
					std::cout << "ID: " << loginResponse.response_struct.id << std::endl;
					std::cout << "Response: " << loginResponse.response_struct.permissions << std::endl;
				}
				else if (action == Protocol::ACTION_LOGOUT_RESPONSE)
				{
					ActionLogout logoutResponse;
					logoutResponse.response_parseToStruct(payload);
					std::cout << "Logout Response" << std::endl;
					std::cout << "" << logoutResponse.response_struct.success << std::endl;
					
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
					else if (answer == 'N' || answer == 'n')
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

		void TCP_Client::connect(int id, bool disconnect_indicator)
		{
			std::cout << "Amount of different books: " << Book::books.size() << " Connect created test book: " << Book::books.find(id)->second.get()->title << ":" << Book::books.find(id)->second.get()->getIsbn() << " ID: " << id << std::endl;

			if (disconnect_indicator)
				disconnect();
			tcp::resolver resolver(socket.get_io_service());
			tcp::resolver::query query(host, Protocol::SERVICE);
			tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
			boost::asio::async_connect(socket, endpoint_iterator, boost::bind(&TCP_Client::handle_connect, this, boost::asio::placeholders::error));
		}

		void TCP_Client::createUser(std::shared_ptr<User> u)
		{

			std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
			std::shared_ptr<char> action_payload;//payload excluding header
			unsigned char action;
			unsigned char following;
			unsigned int payload_size;

			ActionCreateUser createUser;
			createUser.payload_struct.user = u;
			createUser.parseToPayload();
			action_payload = createUser.payload;
			payload_size = createUser.payload_size;
			action = createUser.action;
			following = 0;

			Protocol::encode_header(payload_size, action, following, payload);
			memcpy(payload.get() + Protocol::HEADER_SIZE, action_payload.get(), payload_size);
			payload.get()[Protocol::HEADER_SIZE + payload_size] = '\0';
			std::cout << "input size:" << payload_size << std::endl;

			boost::asio::async_write
			(
				socket,
				boost::asio::buffer(payload.get(), Protocol::HEADER_SIZE + payload_size),
				boost::bind(&TCP_Client::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, action)
			);
		}

		std::shared_ptr<User> TCP_Client::login(User u)
		{
			std::shared_ptr<User> log = NULL;
			User t;
			bool check = false;

			log = u.getUser(u.username);   // log contains pointer from user/map
			// check = u.getPassword().compare(u.getPassword());
			std::cout << u.username << std::endl;
			std::cout << log.get()->username << std::endl;

			if (log.get()->password == u.password)
			{
				//	check = true; 
				return log;
			}

			else
			{
				std::cout << "Login failed. Try again." << std::endl;
				
			}

		}		

		

		
	
	
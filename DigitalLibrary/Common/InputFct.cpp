#include "InputFct.h"
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include "book.h"
#include "User.h"
#include "..\Client\tcp_client.h"




InputFct::InputFct()
{
}


InputFct::~InputFct()
{
}

User InputFct::login()
{

	User u; // make new user object to check with user object in map 
	std::string pw;

	std::cout << "Please login:" << std::endl;
	std::cout << "Username:" << std::endl;
	std::cin >> u.username;
	std::cout << "Password:" << std::endl;
	std::cin >> pw;

	u.password = pw;

	return u;
}

void InputFct::logout()
{
}

void InputFct::quit()
{

}


char InputFct::userScreen(std::shared_ptr<User> u)
{
	
	char choice = '0'; 
	std::shared_ptr<User> check;

	std::cout << "Welcome  " << u.get()->username << "!\n" << std::endl;
	std::cout << "What do you like to do ? \n" << std::endl;
	std::cout << "Enter one of the following:\n" << std::endl;
	std::cout << "'m' for edit your account" << std::endl;
	std::cout << "'s' for searching books in general" << std::endl;
	std::cout << "'i' for searching by the ISBN number" << std::endl;  
	std::cout << "'b' for show your borrowed books" << std::endl;
	std::cout << "'e' for exit / logout" << std::endl;


	std::cin >> choice;
	Book b;
	int bookId;
	std::string isbn; 
	std::map<unsigned int, std::string>::iterator it;

	switch (choice)
	{
	case 'm':

		/*u = modifyUser(u);*/
		return 'm';		  // modifyuser from inputfct

	

	case 's':		
		return 's';


	case 'i':		
			return 'i';
		

	case 'b':		
		return 'b';

	

	case 'e':

		return 'e';

	default:
		std::cout << "Invalid input!" << std::endl;
		break;
	}
	userScreen(u);
}

char InputFct::employeeScreen(std::shared_ptr<User> u)	// Screen with options for the employee
{
	char choice = '0';
	std::string uname; 
	Book b;
	int bookId;
	std::string isbn; 

	std::cout << "Welcome  " << u.get()->username << "!\n" << std::endl;
	std::cout << "What do you like to do ? \n" << std::endl;
	std::cout << "Enter one of the following:\n" << std::endl;
	std::cout << "'m' for edit your account" << std::endl;
	std::cout << "'a' for add a book" << std::endl;
	std::cout << "'c' for modify a book" << std::endl;
	std::cout << "'s' for searching books in general" << std::endl;
	std::cout << "'i' for searching by the ISBN number" << std::endl; // XX
	std::cout << "'z' for show all  books" << std::endl;	
	//std::cout << "'x' for show all borrowed books" << std::endl; 
	std::cout << "'u' for show all users" << std::endl; 
	std::cout << "'e' for exit / logout" << std::endl;

	std::cin >> choice;

	switch (choice)
	{
	case 'm':
		return 'm';	
	

	case 'a':
		return 'a';

	case'c':
		
		return 'c';



	case 's':
		return 's';

	case 'z':
		
		return 'z';

	case 'i':
		
			return 'i';

	//case 'p':
	//	/*std::cout << "Please enter the username to search:" << std::endl;
	//	std::cin >> uname; 
	//	searchUser(uname);*/
	//	return 'p';

	case 'x':
		showAllBorrowedBooks();
		return 'x';

	case'u':
		
		return 'u';


	case 'e':
		return 'e';

	default:
		std::cout << "Invalid input!" << std::endl;
		break;
	}
	userScreen(u);

	return 'e';

}

int InputFct::searchBook()
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
		std::cout << "Do you like to borrow a book?"<< std::endl; 
		std::cout << "Please enter the book ID:" << std::endl;
		std::cout << "(if you do not like to borrow the book enter '0')" << std::endl;
		std::cin >> input;

		return input;

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

		return input;
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

		return input;
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

		return input;
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

		return input;
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

		return input;
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

		return input;
	}

	return 1;
}


int InputFct::searchBookbyISBN(std::string isbn)
{
	std::shared_ptr<Book> b;	
	int input;

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
	std::cin >> input;

	return input; 

}


void InputFct::showTragetBooks(Book b) // or map,list 
{
	std::cout << "Search results:\n" << std::endl;
	// foreach loop when we get a map or list
	// borrow over the iterator
	std::cout << "ISBN:" << "\n" << std::endl;
	std::cout << "Title" << "\n" << std::endl;
	std::cout << "Author" << "\n" << std::endl;
	std::cout << "Summary" << "\n" << std::endl;
	std::cout << "Publisher" << "\n" << std::endl;
	std::cout << "Year" << "\n" << std::endl;
	std::cout << "Amount" << "\n" << std::endl;
	std::cout << "Avaible:" << "\n" << std::endl;

	// ...

	int numberBook;

	std::cout << "Would you like to borrow the / a book?\n" << std::endl;
	std::cout << "Enter the shown number of the book:\n" << std::endl;
	std::cout << "(Do not enter the ISBN number)\n" << std::endl;
	std::cin >> numberBook;

	// borrow logic

}

void InputFct::showBooks() 
{
	Book b;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator it; 
	for (it = b.books.begin(); it != b.books.end(); it++)
	{
		std::cout << "============================================================" << std::endl;
		std::cout << ""                                             << std::endl; 
		std::cout << "Number    :" << it->first                     << std::endl;
		std::cout << "ISBN:	    :" << it->second.get()->getIsbn()   << std::endl;
		std::cout << "Title	    :" << it->second.get()->title       << std::endl;
		std::cout << "Author	:" << it->second.get()->author      << std::endl;
		std::cout << "Summary   :" << it->second.get()->summary     << std::endl;
		std::cout << "Publisher :" << it->second.get()->publisher   << std::endl;
		std::cout << "Year      :" << it->second.get()->year        << std::endl;
		std::cout << "Amount    :" << it->second.get()->getAmount() << std::endl;
		std::cout << ""                                             << std::endl;
		std::cout << "============================================================" << std::endl;
		//std::cout << "Avaible:" << it->second.get()->title << std::endl;
		
	}	
}

void InputFct::showUsers()
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
}


void InputFct::showAllBorrowedBooks()
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
	


}


void InputFct::searchUser(std::string username)
{
	std::map<unsigned int, std::string>::iterator it; 
	std::shared_ptr<User> user; 
	if(user.get()->users.find(username) != user.get()->users.end())
	{
		std::cout << "Username   :" << user.get()->users.find(username)->second->username << std::endl;
		std::cout << "First Name :" << user.get()->users.find(username)->second->fName << std::endl;
		std::cout << "Last Name  :" << user.get()->users.find(username)->second->lName << std::endl;
		std::cout << "City       :" << user.get()->users.find(username)->second->city << std::endl;
		std::cout << "Mobile Phone  :" << user.get()->users.find(username)->second->mPhone << std::endl;
		std::cout << "Number of borrowed books  :" << user.get()->users.find(username)->second->countBorrowedBooks << std::endl;
		std::cout << "User lend following book(s):"<< std::endl; 
		std::cout << "" << std::endl; 

		for (it = user.get()->lendBooks.begin(); it != user.get()->lendBooks.end(); it++)
		{
			std::cout << "BookID		:" << it->first << "Booktitle	:" << it->second << std::endl; 
			std::cout << "" << std::endl;
			std::cout << "===========================================================" << std::endl;
		}
	}

}


//void InputFct::borrowBook(bool success)
//{
//	if (success == 1)
//	{
//		std::cout << "You borrowed the book successfully!" << std::endl;
//		return;
//
//	}
//	else
//	{
//		std::cout << "No more exemplars of this books are available!" << std::endl;
//		return;
//	}

//}

Book InputFct::addBook()
{
	
	std::string temp;
	std::string isbn;
	std::string title;
	std::string author; 
	std::string summary; 
	std::string publisher; 
	int year; 
	int amount;
	char check; 


	std::cout << "Please enter the information for the book, which you like to add:\n" << std::endl;
	std::cout << "ISBN number:" << std::endl;
	std::cin >> isbn;
	std::cout << "Title:" << std::endl;
	std::cin >> title;
	std::cout << "Author:" << std::endl;
	std::cin >> author;
	std::cout << "Summary:" << std::endl;
	std::cin >> summary;
	std::cout << "Publisher:" << std::endl;
	std::cin >> publisher;
	std::cout << "Year:" << std::endl;
	std::cin >> year;
	std::cout << "Amount:" << std::endl;
	std::cin >> amount;
	

	std::shared_ptr<Book> book1 = std::shared_ptr<Book>(new Book());
	book1.get()->constructorBook(title, author, summary, publisher, year, isbn, amount);
	std::cout << book1.get()->title;

	std::cout << " Do you like to add another book to the system?" << std::endl;
	std::cout << "Enter 'y' for yes or 'n' for no" << std::endl;
	std::cin >> check;

	if (check == 'y')
	{
		addBook();
	}


	Book b; 

	return b ;
}


void InputFct::deleteBook()
{
	int deleteId;
	std::cout << "Please enter the id of the book, which you like to delete:" << std::endl;
	std::cin >> deleteId;

	// ..
}

std::shared_ptr<User> InputFct::createUser()
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

	


	return user1;

	// std::cin >>
	// std::cin >>
	// std::cin >>
	// std::cin >>
	// std::cin >>
	// std::cin >>

}

std::shared_ptr<User> InputFct::modifyUser(std::shared_ptr<User> log)
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

	log->modifyUser(fname, lname, nr, city);

	return log;


}


std::shared_ptr<User> InputFct::changePassword(std::shared_ptr<User> log)
{
	std::string old;
	std::string newpw;
	std::string pw;
	bool check;

	std::cout << "\n Enter old password" << std::endl;
	std::cin >> old;
	std::cout << "\n Enter new password" << std::endl;
	std::cin >> newpw;

	pw = log->password;
	check = pw.compare(old);
	if ( pw == old)
	{
		log->password = newpw;
		std::cout << "Your password has been changed." << std::endl;
		return log;
	}
	else
	{
		std::cout << "The old password does not match!" << std::endl;
		return NULL;
	}



}

std::shared_ptr<User> InputFct::deleteUser(std::shared_ptr<User> u)
{
	char check;
	std::cout << "Are you sure?\n Enter y or n" << std::endl;
	std::cin >> check;

	switch (check)
	    case 'y':
	{
		//check here for borrowed books
		if (true)
		{
			// check for borrowed books if no booke left ->
			u->deleteUser(u->username);
			return NULL;
		}
		else
		{
			std::cout << "You cannot delete account when you still have borrowed books" << std::endl;
			// Add here borrowed books?
			break;
		}


		case 'n':
		{
			std::cout << "Nothing happens" << std::endl;
			userScreen(u);
			break;
		}

		return NULL;
	}
}







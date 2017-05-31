#include "InputFct.h"
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include "..\Common\Book.h"
#include "..\Common\user.h"
#include "..\Client\tcp_client.h"



InputFct::InputFct()
{
}


InputFct::~InputFct()
{
}

user InputFct::login()
{
	
	user u; // make new user object to check with user object in map 
	std::string pw; 

	std::cout << "Please login:\n" << std::endl;
	std::cout << "Username:" << std::endl; 
	std::cin >> u.username;
	std::cout << "\nPassword:" << std::endl; 
	std::cin >> pw;

	u.setPassword(pw);

	return u; 
}

void InputFct::logout()
{
}

void InputFct::quit()
{

}


std::tuple<char, std::shared_ptr<user>> InputFct::userScreen(std::shared_ptr<user> u)
{
	char choice = '0'; 
	std::shared_ptr<user> check;

	std::cout << "\n Welcome" << u->getfName() << "!\n" << std::endl; 
	std::cout << "What do you like to do ? \n" << std::endl;
	std::cout << "Enter one of the following:\n" << std::endl;
	std::cout << "'m' for edit your account" << std::endl;
	std::cout << "'c' for change your password" << std::endl; 
	std::cout << "'s' for searching books in general" << std::endl;
	std::cout << "'i' for searching by the ISBN number" << std::endl;
	std::cout << "'b' for show your borrowed books" << std::endl; 
	std::cout << "'d' for delete your account" << std::endl;
	std::cout << "'e' for exit / logout" << std::endl;

	std::cin >> choice;

	switch (choice)
	{
	case 'm':		
		return std::make_tuple('m', modifyUser(u));		  // modifyuser from inputfct

	case'c':
		check = changePassword(u);

		if(check != NULL)
		{
			return std::make_tuple('c', check);
		}
		else
		{
			return std::make_tuple('x', check);				//nothing should happen
		}
		

	case 's':
		searchBook(); 
		

	case 'i':
		

	case 'b':
		// list books
		break; 

	case 'd':
		
		deleteUser(u);
		
		break;

	case 'e':
		return std::make_tuple('e', u); 

	default:
		std::cout << "Invalid input!" << std::endl; 
		break; 
	}
	userScreen(u);
}

char InputFct::employeeScreen(user u)
{
	char choice = 'c';

	std::cout << "Welcome" << u.getfName() << "!\n" << std::endl;
	std::cout << "What do you like to do ? \n" << std::endl;
	std::cout << "Enter one of the following:\n" << std::endl;
	std::cout << "'m' for edit your account" << std::endl;
	std::cout << "'s' for searching books in general" << std::endl;
	std::cout << "'i' for searching by the ISBN number" << std::endl;
	std::cout << "'a' for searching a specific user" << std::endl; 
	std::cout << "'b' for show all borrowed books" << std::endl;
	std::cout << "'u' for show all users" << std::endl;
	std::cout << "'e' for exit / logout" << std::endl;

	std::cin >> choice; 

	return 'c';
}

void InputFct::searchBook()
{
	std::string title;
	std::string author;
	std::string publisher;
	int year;
	Book b;
	std::list<std::shared_ptr<Book>> search;
	std::list<std::shared_ptr<Book>>::iterator it;
	std::shared_ptr<Book > book;



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

			std::cout <<"Title: " << book->title << std::endl;
			std::cout <<"Author: " << book->author << std::endl;
			std::cout <<"Summary: " << book->summary << std::endl;
			std::cout <<"Publisher: " << book->publisher << std::endl; 
			std::cout <<"Year: " << book->year << std::endl; 
			std::cout <<"ISBN: " << book->getIsbn() << std::endl; 
		}
	}
	if (title != "0" && author != "0" && publisher == "0" && year == 0)
	{
		search = b.getBooks(title, author);
		for (it = search.begin(); it != search.end(); it++)
		{
			book = *it;

			std::cout << "Title: " << book->title << std::endl;
			std::cout << "Author: " << book->author << std::endl;
			std::cout << "Summary: " << book->summary << std::endl;
			std::cout << "Publisher: " << book->publisher << std::endl;
			std::cout << "Year: " << book->year << std::endl;
			std::cout << "ISBN: " << book->getIsbn() << std::endl;
		}
	}
	if (title != "0" && author != "0" && publisher != "0" && year == 0)
	{
		search = b.getBooks(title, author, publisher);
		for (it = search.begin(); it != search.end(); it++)
		{
			book = *it;

			std::cout << "Title: " << book->title << std::endl;
			std::cout << "Author: " << book->author << std::endl;
			std::cout << "Summary: " << book->summary << std::endl;
			std::cout << "Publisher: " << book->publisher << std::endl;
			std::cout << "Year: " << book->year << std::endl;
			std::cout << "ISBN: " << book->getIsbn() << std::endl;
		}
	}
	if (title != "0" && author != "0" && publisher != "0" && year != 0)
	{
		search = b.getBooks(title, author, publisher, year);
		for (it = search.begin(); it != search.end(); it++)
		{
			book = *it;

			std::cout << "Title: " << book->title << std::endl;
			std::cout << "Author: " << book->author << std::endl;
			std::cout << "Summary: " << book->summary << std::endl;
			std::cout << "Publisher: " << book->publisher << std::endl;
			std::cout << "Year: " << book->year << std::endl;
			std::cout << "ISBN: " << book->getIsbn() << std::endl;
		}
	}
	if (title == "0" && author != "0" && publisher == "0" && year == 0)
	{
		search = b.getBooksbyAuthor(author);
		for (it = search.begin(); it != search.end(); it++)
		{
			book = *it;

			std::cout << "Title: " << book->title << std::endl;
			std::cout << "Author: " << book->author << std::endl;
			std::cout << "Summary: " << book->summary << std::endl;
			std::cout << "Publisher: " << book->publisher << std::endl;
			std::cout << "Year: " << book->year << std::endl;
			std::cout << "ISBN: " << book->getIsbn() << std::endl;
		}
	}
	if (title == "0" && author != "0" && publisher != "0" && year == 0)
	{
		search = b.getBooksbyPublandAuth(author, publisher);
		for (it = search.begin(); it != search.end(); it++)
		{
			book = *it;

			std::cout << "Title: " << book->title << std::endl;
			std::cout << "Author: " << book->author << std::endl;
			std::cout << "Summary: " << book->summary << std::endl;
			std::cout << "Publisher: " << book->publisher << std::endl;
			std::cout << "Year: " << book->year << std::endl;
			std::cout << "ISBN: " << book->getIsbn() << std::endl;
		}
	}
	if (title == "0" && author == "0" && publisher != "0" && year == 0)
	{
		search = b.getBooksbyPubl(publisher);
		for (it = search.begin(); it != search.end(); it++)
		{
			book = *it;

			std::cout << "Title: " << book->title << std::endl;
			std::cout << "Author: " << book->author << std::endl;
			std::cout << "Summary: " << book->summary << std::endl;
			std::cout << "Publisher: " << book->publisher << std::endl;
			std::cout << "Year: " << book->year << std::endl;
			std::cout << "ISBN: " << book->getIsbn() << std::endl;
		}
	}

	return; 
}



void InputFct::showTragetBooks(Book b) // or map,list 
{
	std::cout << "Search results:\n" << std::endl; 
	// foreach loop when we get a map or list
	// borrow over the iterator
	std::cout << "ISBN:"<<   "\n"<<std::endl;
	std::cout << "Title"<<   "\n" <<std::endl;
	std::cout << "Author"<<  "\n" <<std::endl;
	std::cout << "Summary"<< "\n" <<std::endl;
	std::cout << "Publisher"<< "\n" <<std::endl;
	std::cout << "Year"<<    "\n" <<std::endl;
	std::cout << "Amount"<<  "\n" <<std::endl;
	std::cout << "Avaible:"<<  "\n" << std::endl;

	// ...
	
	int numberBook;

	std::cout << "Would you like to borrow the / a book?\n" << std::endl; 
	std::cout << "Enter the shown number of the book:\n" << std::endl; 
	std::cout << "(Do not enter the ISBN number)\n" << std::endl; 
	std::cin >> numberBook; 

	// borrow logic

}

void InputFct::borrowBook(bool success)
{
	if (success == 1)
	{
		std::cout << "You borrowed the book successfully!" << std::endl;
		return;
		
	}
	else
	{
		std::cout << "No more exemplars of this books are available!" << std::endl; 
		return;
	}

}

Book InputFct::addBook()
{
	Book b = Book();
	std::string temp; 
	int temp1; 

	std::cout << "Please enter the information for the book, which you like to add:\n" << std::endl; 
	std::cout << "ISBN number:" << std::endl; 
	std::cin >>  temp; 
    b.setIsbn(temp); 
	std::cout << "Title:" << std::endl;
	std::cin >> b.title;
	std::cout << "Author:" << std::endl;
	std::cin >> b.author; 
	std::cout << "Summary:" << std::endl;
	std::cin >> b.summary; 
	std::cout << "Publisher:" << std::endl;
	std::cin >> b.publisher; 
	std::cout << "Year:" << std::endl;
	std::cin >> b.year; 
	std::cout << "Amount:" << std::endl;
	std::cin >> temp1;
	b.setAmount(temp1);

	return b; 
}


void InputFct::deleteBook()
{
	int deleteId;
	std::cout << "Please enter the id of the book, which you like to delete:" << std::endl;
	std::cin >> deleteId;

	// ..
}

user InputFct::createUser()
{
	
	std::string username; 
	std::string pw;

	std::cout << "Please enter the necessary data to create an account:\n" << std::endl;
	std::cout << "Username:" << std::endl;
	std::cin >> username;
	std::cout << "\n Password:\n" << std::endl;
	std::cout << "(With no entry a standard password will be set):" << std::endl;
	std::cin >> pw;

	user u = user(1,username,pw);
	
	return u; 

	// std::cin >>
	// std::cin >>
	// std::cin >>
	// std::cin >>
	// std::cin >>
	// std::cin >>
	
}

std::shared_ptr<user> InputFct::modifyUser(std::shared_ptr<user> log)
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


std::shared_ptr<user> InputFct::changePassword(std::shared_ptr<user> log) 
{
	std::string old;
	std::string newpw;
	std::string pw;
	bool check;

	std::cout << "\n Enter old password" << std::endl;
	std::cin >> old;
	std::cout << "\n Enter new password" << std::endl;
	std::cin >> newpw;

	pw = log->getPassword();
	check = pw.compare(old);

	if (check == true)
	{
		log->setPassword(newpw);
		return log;
	}
	else
	{
		std::cout << "The old password does not match!" << std::endl;	
		return NULL; 
	}

	

}

std::shared_ptr<user> InputFct::deleteUser(std::shared_ptr<user> u)
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
			break;
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







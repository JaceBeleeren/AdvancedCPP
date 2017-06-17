#include "book.h"
#include "User.h"

std::map<unsigned int, std::shared_ptr<Book>> Book::books;
//constructor only sets constructed to false to indicate that constructorBook() still needs to be called
//only call constructor like this:
//std::shared_ptr<Book> book = std::shared_ptr<Book>(new Book());
//then right after calling the constructor, immediatly call one (and only one!) of the actual constructors:
//book.get()->constructorBook(isbn);
//book.get()->constructorBook(title, author, summary, publisher, year, isbn, amount);
Book::Book() {
	constructed = false;
}

//actual constructor, call right after calling constructor
void Book::constructorBook(std::string isbn)
{
	std::string errorMessage = "";
	checkConstruction(false);
	unsigned int tempId = 1;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator it;
	for (it  = books.begin(); it != books.end(); it++)
	{
		if (it->first >= tempId)
			tempId++;
		if (isbn == it->second.get()->getIsbn())
		{
			errorMessage += "\nISBN " + isbn + " already exists!";
			std::cout << errorMessage;
			throw std::invalid_argument(errorMessage);
		}
	}

	this->id = tempId;
	this->title = "";
	this->author = "";
	this->summary = "";
	this->publisher = "";
	this->year = 0;
	this->isbn = isbn;
	this->amount = 0;
	this->borrowed = 0;
	books.insert(std::make_pair(id, shared_from_this()));
	constructed = true;
}

//actual constructor, call right after calling constructor
void Book::constructorBook(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount)
{
	std::string errorMessage = "";
	checkConstruction(false);
	unsigned int tempId = 1;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator it;
	for (it = books.begin(); it != books.end(); it++)
	{
		if (it->first >= tempId)
			tempId++;
		if (isbn == it->second.get()->getIsbn())
		{
			errorMessage += "\nISBN " + isbn + " already exists!";
			std::cout << errorMessage;
			throw std::invalid_argument(errorMessage);
		}
	}

	this->id = tempId;
	this->title = title;
	this->author = author;
	this->summary = summary;
	this->publisher = publisher;
	this->year = year;
	this->isbn = isbn;
	this->amount = amount;
	this->borrowed = 0;
	books.insert(std::make_pair(id, shared_from_this()));
	constructed = true;
}



Book::~Book()
{
}

std::string Book::modifyBook(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount, unsigned int id)  // static
{
	std::string errorMessage = "";
	if (books.find(id) == books.end())
	{
		
		errorMessage += ("\nBook with given id" + std::to_string(id) + " does not exist!");
		return errorMessage;
	}
	std::shared_ptr<Book> book = books.find(id)->second;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator it;
	for (it = books.begin(); it != books.end(); it++)
	{
		if (it->first != id && isbn == it->second.get()->getIsbn())
		{
			errorMessage += "\nISBN " + isbn + " already exists!";
			return errorMessage;
		}
	}
	if (amount < book.get()->borrowed)
		book.get()->borrowed = amount;
	book.get()->title = title;
	book.get()->author = author;
	book.get()->summary = summary;
	book.get()->publisher = publisher;
	book.get()->year = year;
	book.get()->isbn = isbn;
	book.get()->amount = amount;

	return errorMessage;
}

std::string Book::modifyBook(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount)
{
	std::string errorMessage = "";
	//checkConstruction(true);
	return modifyBook(title, author, summary, publisher, year, isbn, amount, this->id);
}

std::string Book::deleteBook(unsigned int id)
{
	std::string errorMessage = "";
	if (books.find(id) == books.end())
	{
		errorMessage += ("\nBook with given id" + std::to_string(id) + " does not exist!");
		return errorMessage;
	}
	books.erase(id);
	return errorMessage;
}

std::string Book::deleteBook()
{
	std::string errorMessage = "";
	checkConstruction(true);
	return deleteBook(this->id);
}

std::shared_ptr<Book> Book::getBook(unsigned int id)
{
	std::string errorMessage = "";
	if (books.find(id) == books.end())
	{
		errorMessage += ("\nBook with given id" + std::to_string(id) + " does not exist!");
		throw std::invalid_argument(errorMessage);
	}
	std::shared_ptr<Book> book = books.find(id)->second;
	return book;
}

//std::map<unsigned int, std::tuple<unsigned int, unsigned int>> Book::getBorrowedBooks()
//{
//	std::map<unsigned int, std::tuple<unsigned int, unsigned int>> borrowedBooks;
//	std::map<unsigned int, std::shared_ptr<Book>>::iterator itBooks;
//	for (itBooks = books.begin(); itBooks != books.end(); itBooks++)
//	{
//		std::shared_ptr<Book> book = itBooks->second;
//		std::map<unsigned int, unsigned int>::iterator itBorrowed;
//		for (itBorrowed = book.get()->borrowedByUser.begin(); itBorrowed != book.get()->borrowedByUser.end(); itBorrowed++)
//		{
//			if (itBorrowed->second > 0)
//				borrowedBooks.insert(std::make_pair(itBooks->first, std::make_tuple(itBorrowed->first, itBorrowed->second)));//bookId/key, <userId, amount>
//		}
//	}
//	return borrowedBooks;
//}

std::shared_ptr<Book> Book::getBookbyISBN(std::string isbn)
{
	std::shared_ptr<Book> b; 
	std::map<unsigned int, std::shared_ptr<Book>>::iterator itBooks;

	for (itBooks = books.begin(); itBooks != books.end(); itBooks++)
	{
		if (itBooks->second.get()->getIsbn() == isbn)
		{
			b = itBooks->second;
		}
	}
	return b ;
}


std::list <std::shared_ptr<Book>> Book::getBooks(std::string title)  // by Title
{
	//std::map<unsigned int, std::tuple<std::string, unsigned int>> searchedBooks;
	std::list <std::shared_ptr<Book>> searchedBooks;
	std::list<std::shared_ptr<Book>>::iterator it;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator itBooks;
	
	for (itBooks = books.begin(); itBooks != books.end(); itBooks++)
	{
		
		it = searchedBooks.begin();
	
		
			if (itBooks->second.get()->title == title)
			{
				searchedBooks.insert(it, itBooks->second);
			}
		
		/*++it;*/

		//std::map<std::string, unsigned int>::iterator itSearched;
		//for (itSearched = book.get()->borrowedByUser.begin(); itSearched != book.get()->borrowedByUser.end(); itSearched++)
		//{
		//	if (itBorrowed->second > 0)
		//		borrowedBooks.insert(std::make_pair(itBooks->first, std::make_tuple(itBorrowed->first, itBorrowed->second)));//bookId/key, <userId, amount>
		//}
	}
	//it = searchedBooks.end(); 
	return searchedBooks;
}

std::list <std::shared_ptr<Book>> Book::getBooksbyAuthor(std::string author)  // by author
{
	//std::map<unsigned int, std::tuple<std::string, unsigned int>> searchedBooks;
	std::list <std::shared_ptr<Book>> searchedBooks;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator itBooks;
	std::list<std::shared_ptr<Book>>::iterator it;
	for (itBooks = books.begin(); itBooks != books.end(); itBooks++)
	{

		it = searchedBooks.begin();


		if (itBooks->second.get()->author == author)
		{
			searchedBooks.insert(it, itBooks->second);
		}	

	}
	return searchedBooks;
}

std::list <std::shared_ptr<Book>> Book::getBooks(std::string title, std::string author)  // by Title and author
{
	//std::map<unsigned int, std::tuple<std::string, unsigned int>> searchedBooks;
	std::list <std::shared_ptr<Book>> searchedBooks;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator itBooks;
	std::list<std::shared_ptr<Book>>::iterator it;
	for (itBooks = books.begin(); itBooks != books.end(); itBooks++)
	{

		it = searchedBooks.begin();

		if (itBooks->second.get()->title == title && itBooks->second.get()->author == author)
		{
			searchedBooks.insert(it, itBooks->second);
		}

	}
	return searchedBooks;
}

std::list <std::shared_ptr<Book>> Book::getBooks(std::string title, std::string author, std::string publisher)  // by Title and author and publisher
{
	//std::map<unsigned int, std::tuple<std::string, unsigned int>> searchedBooks;
	std::list <std::shared_ptr<Book>> searchedBooks;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator itBooks;
	std::list<std::shared_ptr<Book>>::iterator it;
	for (itBooks = books.begin(); itBooks != books.end(); itBooks++)
	{

		it = searchedBooks.begin();

		if (itBooks->second.get()->title == title && itBooks->second.get()->author == author && itBooks->second.get()->publisher == publisher)
		{
			searchedBooks.insert(it, itBooks->second);
		}

	}
	return searchedBooks;
}


std::list <std::shared_ptr<Book>> Book::getBooks(std::string title, std::string author, std::string publisher, int year)  // by Title and author and publisher
{
	//std::map<unsigned int, std::tuple<std::string, unsigned int>> searchedBooks;
	std::list <std::shared_ptr<Book>> searchedBooks;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator itBooks;
	std::list<std::shared_ptr<Book>>::iterator it;
	for (itBooks = books.begin(); itBooks != books.end(); itBooks++)
	{

		it = searchedBooks.begin();

		if (itBooks->second.get()->title == title && itBooks->second.get()->author == author 
			&& itBooks->second.get()->publisher == publisher && itBooks->second.get()->year == year)
		{
			searchedBooks.insert(it, itBooks->second);
		}


	}
	return searchedBooks;
}

std::list <std::shared_ptr<Book>> Book::getBooksbyPubl(std::string publisher)
{
	//std::map<unsigned int, std::tuple<std::string, unsigned int>> searchedBooks;
	std::list <std::shared_ptr<Book>> searchedBooks;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator itBooks;
	std::list<std::shared_ptr<Book>>::iterator it;
	for (itBooks = books.begin(); itBooks != books.end(); itBooks++)
	{

		it = searchedBooks.begin();

		if (itBooks->second.get()->publisher == publisher)
		{
			searchedBooks.insert(it, itBooks->second);
		}

	}
	return searchedBooks;
}


std::list <std::shared_ptr<Book>> Book::getBooksbyPublandAuth(std::string author, std::string publisher)
{
	//std::map<unsigned int, std::tuple<std::string, unsigned int>> searchedBooks;
	std::list <std::shared_ptr<Book>> searchedBooks;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator itBooks;
	std::list<std::shared_ptr<Book>>::iterator it;
	for (itBooks = books.begin(); itBooks != books.end(); itBooks++)
	{

		it = searchedBooks.begin();

		if (itBooks->second.get()->publisher == publisher && itBooks->second.get()->author == author)
		{
			searchedBooks.insert(it, itBooks->second);
		}

	}
	return searchedBooks;
}


std::shared_ptr<Book> Book::getBooks(int bookId)
{
	std::shared_ptr<Book> b;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator itBooks;
	for (itBooks = books.begin(); itBooks != books.end(); itBooks++)
	{
		if (itBooks->second.get()->getId() == bookId)
		{
			 b = itBooks->second;
		}
	}
	return b;
}




std::string Book::borrow(unsigned int id, std::shared_ptr<User> user)
{
	std::map<unsigned int, std::shared_ptr<Book>>::iterator it; 
	std::string errorMessage = "";
	if (books.find(id) == books.end())
	{
		errorMessage += "\nBook with given id" + std::to_string(id) + " does not exist!";
	}
	
	std::shared_ptr<Book> book = books.find(id)->second;
	if (book.get()->borrowed >= book.get()->amount)
	{
		errorMessage += "\nNo book left";
	}

	if (errorMessage == "")
	{
		if (book.get()->borrowedByUser.find(user->username) != book.get()->borrowedByUser.end())  // user is in the list
		{
			book.get()->borrowedByUser.find(user->username)->second += 1;			
		}
		else
		{
			book.get()->borrowedByUser.insert(std::make_pair(user->username, 1));  // user insert into list
			book.get()->borrowed += 1;
		}
		
		user.get()->countBorrowedBooks = +1; 
		user.get()->lendBooks.insert(make_pair(book.get()->getId(), book.get()->title));

	}
	return errorMessage;
}

///*std::string Book::borrow(std::string username)
//{
//	std::string errorMessage = "";
//	checkConstruction(true);
//	*/return borrow(this->id, username);
//}

std::string Book::returnBook(unsigned int id, std::string username)
{
	std::string errorMessage = "";
	if (books.find(id) == books.end())
	{
		errorMessage += "\nBook with given id" + std::to_string(id) + " does not exist!";
	}
	//check if user exists
	/*if (User::users.find(userId) == User::users.end())
	{
	errorMessage += "\nUser with given id" + std::to_string(id) + " does not exist!";
	}
	*/
	std::shared_ptr<Book> book = books.find(id)->second;
	if (book.get()->borrowedByUser.find(username) == book.get()->borrowedByUser.end())
	{
		errorMessage += "\nUser with id " + username + " does not have borrowed Book with id " + std::to_string(id);
	}
	else if(book.get()->borrowedByUser.find(username)->second <= 0)
	{
		errorMessage += "\nUser with id " + username + " does not have borrowed Book with id " + std::to_string(id);
	}
	if (errorMessage == "")
	{
		book.get()->borrowedByUser.find(username)->second -= 1;
		if (book <= 0)//one book returned which should not exists. why not er are happy with that
			book.get()->amount++;
		else
			book.get()->borrowed--;
	}
	return errorMessage;
}

//std::string Book::returnBook(std::string username)
//{
//	checkConstruction(true);
//	return returnBook(this->id, userId);
//}

unsigned int Book::getId()
{
	checkConstruction(true);
	return this->id;
}

const std::string& Book::getIsbn()
{
	checkConstruction(true);
	return this->isbn;
}

std::string Book::setIsbn(std::string isbn)
{
	checkConstruction(true);
	std::string errorMessage = "";
	std::map<unsigned int, std::shared_ptr<Book>>::iterator it;
	for (it = books.begin(); it != books.end(); it++)
	{
		if (it->first != this->id && isbn == it->second.get()->getIsbn())
		{
			errorMessage += "\nISBN " + isbn + " already exists!";
			return errorMessage;
		}
	}
	this->isbn = isbn;
	return errorMessage;
}

unsigned int Book::getAmount()
{
	checkConstruction(true);
	return this->amount;
}

std::string Book::setAmount(unsigned int amount)
{
	checkConstruction(true);
	std::string message = "";
	if (amount < this->borrowed)
	{
		message += "\nBorrowed books also decreased from " + std::to_string(borrowed) + " to " + std::to_string(amount);
		this->borrowed = amount;
	}
	this->amount = amount;
	return message;
}

unsigned int Book::getBorrowed()
{
	//checkConstruction(true);
	return this->borrowed;
}

bool Book::checkConstruction(bool construction)
{
	std::string errorMessage = "";
	if (construction && !constructed)
	{
		errorMessage += "\nconstructorBook() was not called yet!";
		std::cout << errorMessage;
		throw std::exception(errorMessage.c_str());
		return false;
	}
	else if(!construction && constructed)
	{
		errorMessage += "\nconstructorBook() was already called!";
		std::cout << errorMessage;
		throw std::exception(errorMessage.c_str());
		return false;
	}
	return true;
}




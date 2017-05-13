#include "book.h"

std::map<unsigned int, std::shared_ptr<Book>> Book::books;
Book::Book(std::string isbn)
{
	std::string errorMessage = "";
	unsigned int tempId = 1;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator it;
	for (it = books.begin(); it != books.end(); it++)
	{
		if (it->first >= tempId)
			tempId++;
		if (isbn == it->second.get()->getIsbn())
		{
			errorMessage += "\nISBN already exists!";
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
	books.insert(std::make_pair(id, std::shared_ptr<Book>(this)));
}

Book::Book(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount)
{
	std::string errorMessage = "";
	unsigned int tempId = 1;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator it;
	for (it = books.begin(); it != books.end(); it++)
	{
		if (it->first >= tempId)
			tempId++;
		if (isbn == it->second.get()->getIsbn())
		{
			errorMessage += "\nISBN already exists!";
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
	books.insert(std::make_pair(id, std::shared_ptr<Book>(this)));
}

Book::~Book()
{
}

std::string Book::modifyBook(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount, unsigned int id)
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

std::map<unsigned int, std::tuple<unsigned int, unsigned int>> Book::getBorrowedBooks()
{
	std::map<unsigned int, std::tuple<unsigned int, unsigned int>> borrowedBooks;
	std::map<unsigned int, std::shared_ptr<Book>>::iterator itBooks;
	for (itBooks = books.begin(); itBooks != books.end(); itBooks++)
	{
		std::shared_ptr<Book> book = itBooks->second;
		std::map<unsigned int, unsigned int>::iterator itBorrowed;
		for (itBorrowed = book.get()->borrowedByUser.begin(); itBorrowed != book.get()->borrowedByUser.end(); itBorrowed++)
		{
			if (itBorrowed->second > 0)
				borrowedBooks.insert(std::make_pair(itBooks->first, std::make_tuple(itBorrowed->first, itBorrowed->second)));//bookId/key, <userId, amount>
		}
	}
	return borrowedBooks;
}

std::string Book::borrow(unsigned int id, unsigned int userId)
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
	if (book.get()->borrowed >= book.get()->amount)
	{
		errorMessage += "\nNo book left";
	}
	if (errorMessage == "")
	{
		if (book.get()->borrowedByUser.find(userId) != book.get()->borrowedByUser.end())
		{
			book.get()->borrowedByUser.find(userId)->second += 1;
		}
		else
			book.get()->borrowedByUser.insert(std::make_pair(userId, 1));
		book.get()->borrowed += 1;
	}
	return errorMessage;
}

std::string Book::borrow(unsigned int userId)
{
	return borrow(this->id, userId);
}

std::string Book::returnBook(unsigned int id, unsigned int userId)
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
	if (book.get()->borrowedByUser.find(userId) == book.get()->borrowedByUser.end())
	{
		errorMessage += "\nUser with id " + std::to_string(userId) + " does not have borrowed Book with id " + std::to_string(id);
	}
	else if(book.get()->borrowedByUser.find(userId)->second <= 0)
	{
		errorMessage += "\nUser with id " + std::to_string(userId) + " does not have borrowed Book with id " + std::to_string(id);
	}
	if (errorMessage == "")
	{
		book.get()->borrowedByUser.find(userId)->second -= 1;
		if (book <= 0)//one book returned which should not exists. why not er are happy with that
			book.get()->amount++;
		else
			book.get()->borrowed--;
	}
	return errorMessage;
}

std::string Book::returnBook(unsigned int userId)
{
	return returnBook(this->id, userId);
}

unsigned int Book::getId()
{
	return this->id;
}

const std::string& Book::getIsbn()
{
	return this->isbn;
}

std::string Book::setIsbn(std::string isbn)
{
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
	return this->amount;
}

std::string Book::setAmount(unsigned int amount)
{
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
	return this->borrowed;
}


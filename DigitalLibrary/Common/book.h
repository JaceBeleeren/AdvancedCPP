#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <map>
#include <string>
#include <tuple>
#include <functional>
#include "User.h"

class Book : public std::enable_shared_from_this<Book>
{
public:
	Book();//only call constructor like this std::shared_ptr<Book> book = std::shared_ptr<Book>(new Book());! call constructorBook() after calling the constructor!
	bool constructed;
	void constructorBook(std::string isbn);//throws exception std::invalid_argument //actual constructor
	void constructorBook(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount = 0);//throws exception std::invalid_argument //actual constructor
	~Book();
	static std::string modifyBook(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount, unsigned int id);
	std::string modifyBook(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount);
	static std::string deleteBook(unsigned int id);
	std::string deleteBook();

	static std::map<unsigned int, std::shared_ptr<Book>> books; //list of all available books
	static std::shared_ptr<Book> getBook(unsigned int id); //throws exception std::invalid_argument
	//static std::map<unsigned int, std::tuple<unsigned int, unsigned int>> getBorrowedBooks(); //key = bokId, first value = userId, second value = amount


	static std::list <std::shared_ptr<Book>> Book::getBooks(std::string title);  // search by title
	static std::list <std::shared_ptr<Book>> Book::getBooksbyAuthor(std::string author);
	static std::list <std::shared_ptr<Book>> Book::getBooks(std::string title, std::string author);
	static std::list <std::shared_ptr<Book>> Book::getBooks(std::string title, std::string author, std::string publisher);  // by Title and author and publisher
	static std::list <std::shared_ptr<Book>> Book::getBooks(std::string title, std::string author, std::string publisher, int year);  // by Title and author and publisher
	static std::list <std::shared_ptr<Book>> Book::getBooksbyPubl(std::string publisher);
	static std::list <std::shared_ptr<Book>> Book::getBooksbyPublandAuth(std::string author, std::string publisher);
	static std::shared_ptr<Book> Book::getBookbyISBN(std::string isbn);
	std::shared_ptr<Book> Book::getBooks(int bookId);  // search by Id

	//void Book::borrowBook(std::shared_ptr<User> user, int id);


	static std::string borrow(unsigned int id, std::shared_ptr<User>);
	std::string borrow(std::string username);
	static std::string returnBook(unsigned int id, std::string username);
	std::string returnBook(unsigned int userId);

	unsigned int getId();
	std::string title;
	std::string author;
	std::string summary;
	std::string publisher;
	unsigned int year;
	const std::string& getIsbn();
	std::string setIsbn(std::string isbn);
	unsigned int getAmount(); //get amount of Books availabale including borrowed books
	std::string setAmount(unsigned int amount); //set amount of Books availabale including borrowed books, if new amount is smaller than borrowed, borrowed will also be decreased
	unsigned int getBorrowed(); //get amount of Books borrowed
	//std::string setBorrowed(); //set amount of books borrowed, cant be higher than amount //not needed since books will be borrowed with borrow()
	std::map<std::string, unsigned int> borrowedByUser;//key = userId, value = amount
	unsigned int id; //Book ID and Key for std::map Books
private:
//bool checkConstruction(bool construction = true);//check if actual constructor was already called (false) or is not called yet (true) and throws exception if its true
	
	std::string isbn;
	unsigned int amount;
	unsigned int borrowed;
};


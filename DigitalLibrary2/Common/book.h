#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <map>
#include <string>
#include <tuple>

class Book
{
public:
	//Book();
	Book(); 
	Book(std::string isbn);//throws exception std::invalid_argument
	Book(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount = 0);//throws exception std::invalid_argument
	~Book();

	static std::string modifyBook(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount, unsigned int id);
	std::string modifyBook(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount);
	static std::string deleteBook(unsigned int id);
	std::string deleteBook();

	static std::map<unsigned int, std::shared_ptr<Book>> books; //list of all available books
	static std::shared_ptr<Book> getBook(unsigned int id); //throws exception std::invalid_argument
	static std::map<unsigned int, std::tuple<std::string, unsigned int>> getBorrowedBooks(); //key = bokId, first value = userId, second value = amount
	static std::list <std::shared_ptr<Book>> Book::getBooks(std::string title);  // search by title
	static std::list <std::shared_ptr<Book>> Book::getBooksbyAuthor(std::string author);
	static std::list <std::shared_ptr<Book>> Book::getBooks(std::string title, std::string author);
	static std::list <std::shared_ptr<Book>> Book::getBooks(std::string title, std::string author, std::string publisher);  // by Title and author and publisher
	static std::list <std::shared_ptr<Book>> Book::getBooks(std::string title, std::string author, std::string publisher, int year);  // by Title and author and publisher
	static std::list <std::shared_ptr<Book>> Book::getBooksbyPubl(std::string publisher);
	static std::list <std::shared_ptr<Book>> Book::getBooksbyPublandAuth(std::string author, std::string publisher);


	static std::string borrow(unsigned int id, std::string username);
	std::string borrow(std::string username);
	static std::string returnBook(unsigned int id, std::string username);
	std::string returnBook(std::string userame);

	Book addBook(Book b);

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
	std::map<std::string, unsigned int> borrowedByUser;//key = username, value = amount
private:
	unsigned int id; //Book ID and Key for std::map Books
	std::string isbn;
	unsigned int amount;
	unsigned int borrowed;
};


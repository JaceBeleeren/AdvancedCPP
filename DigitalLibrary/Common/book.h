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
	Book(std::string isbn);//throws exception std::invalid_argument
	Book(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount = 0);//throws exception std::invalid_argument
	~Book();
	static std::string modifyBook(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount, unsigned int id);
	std::string modifyBook(std::string title, std::string author, std::string summary, std::string publisher, unsigned int year, std::string isbn, unsigned int amount);
	static std::string deleteBook(unsigned int id);
	std::string deleteBook();

	static std::map<unsigned int, std::shared_ptr<Book>> books; //list of all available books
	static std::shared_ptr<Book> getBook(unsigned int id); //throws exception std::invalid_argument
	static std::map<unsigned int, std::tuple<unsigned int, unsigned int>> getBorrowedBooks(); //key = bokId, first value = userId, second value = amount

	static std::string borrow(unsigned int id, unsigned int userId);
	std::string borrow(unsigned int userId);
	static std::string returnBook(unsigned int id, unsigned int userId);
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
	std::map<unsigned int, unsigned int> borrowedByUser;//key = userId, value = amount
private:
	unsigned int id; //Book ID and Key for std::map Books
	std::string isbn;
	unsigned int amount;
	unsigned int borrowed;
};


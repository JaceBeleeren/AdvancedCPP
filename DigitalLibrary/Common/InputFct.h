#pragma once
#include <iostream>
#include "User.h"
#include "book.h"


static class InputFct
{
public:
	InputFct();
	~InputFct();

	User login();
	void logout();
	void quit();

	char userScreen(std::shared_ptr<User> u);
	char employeeScreen(std::shared_ptr<User> u);


	void InputFct::showUsers();
	int searchBook();
	int searchBookbyISBN(std::string isbn);
	void showBooks();
	void showTragetBooks(Book b);
	void showAllBorrowedBooks();
	Book addBook();
	void deleteBook();

	std::shared_ptr<User> createUser();
	std::shared_ptr<User> modifyUser(std::shared_ptr<User> log);
	std::shared_ptr<User> InputFct::changePassword(std::shared_ptr<User> log);
	void searchUser(std::string username);
	std::shared_ptr<User> InputFct::deleteUser(std::shared_ptr<User> u);

};


#pragma once
#include <iostream>
#include "user.h"
#include "..\Common\book.h"

static class InputFct
{
public:
	InputFct();
	~InputFct();

	user login();
	void logout();
	void quit();

	std::tuple<char, std::shared_ptr<user>> userScreen(std::shared_ptr<user> u);
	char employeeScreen(user u); 

	void searchBook(); 
	void showTragetBooks(Book b);
	void borrowBook(bool succes); 
	Book addBook(); 
	void deleteBook();

	user createUser();
	std::shared_ptr<user> modifyUser(std::shared_ptr<user> log);
	std::shared_ptr<user> InputFct::changePassword(std::shared_ptr<user> log);
	void modifyUserEmployee();
	void searchUser(); 
	std::shared_ptr<user> InputFct::deleteUser(std::shared_ptr<user> u);
	
	

};


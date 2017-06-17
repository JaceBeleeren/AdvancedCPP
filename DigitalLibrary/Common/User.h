#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <map>
#include <string>
#include <tuple>




class User : public std::enable_shared_from_this<User>
{
public:
	User();//only call constructor like this std::shared_ptr<User> book = std::shared_ptr<Book>(new User());! call constructorUser() after calling the constructor!
	bool constructed; 
	void constructorUser(int type, std::string username, std::string pw );
	
	//User(int type, std::string username, std::string pw = "1234");
	//User(int type, std::string username, std::string firstName, std::string lastName, std::string pw = "1234");
	//User(int type, std::string username, std::string firstName, std::string lastName, std::string pw, int mPhone, std::string city);
	~User();


	std::map<unsigned int, std::string> lendBooks;		// bookId and title
	// static std::shared_ptr<user> getUser(unsigned int userId);
	static std::shared_ptr<User> getUser(std::string username);


	static std::string modifyUser(std::string username, std::string firstName, std::string lastName, int mPhone, std::string city);
	std::string modifyUser(std::string firstName, std::string lastName, int mPhone, std::string city);
	int changePassword(User user, std::string oldPassword, std::string newPassword);

	static std::string deleteUser(std::string username);
	std::string deleteUser();

	static std::map<std::string, std::shared_ptr<User>> users;
	static std::map<unsigned int, std::tuple<unsigned int, unsigned int>> getLendBooks(); //key = username, first value = bookId, second value = amount

	

	std::string username;
	int userType;			// 0 = employee 1 = user
	std::string password;
	int countBorrowedBooks;

	std::string fName;
	std::string lName;
	std::string city;
	int mPhone;
	

private:
	

};


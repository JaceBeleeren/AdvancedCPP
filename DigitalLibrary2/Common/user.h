#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <map>
#include <string>
#include <tuple>


class user
{
public:
	user();
	user(int type, std::string username, std::string pw = "1234");
	user(int type, std::string username, std::string firstName, std::string lastName, std::string pw = "1234");
	user(int type, std::string username, std::string firstName, std::string lastName, std::string pw, int mPhone, std::string city);
	~user();

	
	static std::map<std::string, std::shared_ptr<user>> users;
	// static std::shared_ptr<user> getUser(unsigned int userId);
	static std::shared_ptr<user> getUser(std::string username); 

	void modifyUser(std::string firstName, std::string lastName, int mPhone, std::string city);
	int changePassword(user user, std::string oldPassword, std::string newPassword);

	static std::string deleteUser(std::string username);
	std::string deleteUser(); 

	static std::map<unsigned int, std::tuple<unsigned int, unsigned int>> getLendBooks(); //key = username, first value = bookId, second value = amount
	
	

	// get 
	//unsigned int  getId(); 
	int getuserType(); 
	std::string getfName(); 
	std::string  getlName(); 
	std::string  getPassword(); 

	// set 

	//void setId(); 
	void setUsertype(int tId); 
	void setfName(std::string fName); 
	void setlName(std::string lName); 
	void setPassword(std::string password); 

	std::string username;
	int mPhone; 
	std::string city; 


private:
	unsigned int userId;  
	
	int userType;					// 0 = employee 1 = user
	std::string fName;
	std::string lName;
	std::string password; 
	


};


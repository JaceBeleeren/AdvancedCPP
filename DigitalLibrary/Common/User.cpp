#include "User.h"

std::map<std::string, std::shared_ptr<User>> User::users;

User::User()
{
	constructed = false;
}


void User::constructorUser(int type, std::string username, std::string pw)
{
	//checkConstruction(false);
	std::string errorMessage = "";
	std::map<std::string, std::shared_ptr<User>>::iterator it;


	for (it = users.begin(); it != users.end(); it++)
	{		
		if (username == it->second.get()->username)
		{
			errorMessage += "\nUsername " + username + " already exists!";
			std::cout << errorMessage;
			throw std::invalid_argument(errorMessage);
		}
	}
	this->username = username;
	this->password = pw;
	this->userType = type;
	this->countBorrowedBooks = 0; 
	this->fName = "not set yet!";
	this->lName = "not set yet!";
	this->mPhone = 0; 
	this->city = "not set yet!";
	users.insert(std::make_pair(username, shared_from_this()));
	constructed = true;
	
}

//User::User(int utype, std::string username, std::string pw) /*Constructor for create a complete new "account"*/
//{
//	User::userType = utype;
//	this->username = username;
//	setPassword(pw);
//
//
//
//	std::cout << "constructor" << std::endl;
//	std::cout << username << std::endl;
//	users.insert(std::make_pair(username, std::shared_ptr<User>(this)));
//	std::cout << users.find(username)->second.get()->username << std::endl;
//	std::cout << "constructor" << std::endl;
//}

User::~User()
{
}



std::string User::modifyUser(std::string username, std::string firstName, std::string lastName, int mPhone, std::string city)
{
	std::string errorMessage = "";
	if (users.find(username) == users.end())
	{
		errorMessage += ("User with given Username" + username + " does not exist!");
		return errorMessage;
	}
	std::shared_ptr<User> user = users.find(username)->second;
	std::map<std::string, std::shared_ptr<User>>::iterator it; 
	user.get()->fName = firstName;
	user.get()->lName = lastName; 
	user.get()->city = city; 
	user.get()->mPhone = mPhone; 

	return errorMessage;
}

std::string User::modifyUser(std::string firstName, std::string lastName, int mPhone, std::string city)
{
	std::string errorMessage = "";	

	return modifyUser(this->username, firstName, lastName, mPhone, city);
}

int User::changePassword(User user, std::string oldPassword, std::string newPassword)
{
	std::string oldPw = user.password;

	if (oldPw == oldPassword)
	{
		//user.setPassword(newPassword);
		user.password = newPassword;

		return 1;
	}

	else
	{
		std::cout << "The old password does not match!" << std::endl;
		return 0;
	}
}

std::string User::deleteUser(std::string username)
{

	std::string errorMessage = "";
	if (users.find(username) == users.end())
	{
		errorMessage += ("\n User with given Username" + username + " does not exist!");
		return errorMessage;
	}
	users.erase(username);

	return errorMessage;
}

std::string User::deleteUser()
{
	return deleteUser(this->username);
}


//std::shared_ptr<user> user::getUser(unsigned int userId)
//{
//	std::string errorMessage = "";
//	if (users.find(userId) == users.end())
//	{
//		errorMessage += ("\nUser with given UserID" + std::to_string(userId) + " does not exist!");
//		throw std::invalid_argument(errorMessage);
//	}
//	std::shared_ptr<user> user1 = users.find(userId)->second;
//	return user1;
//}

std::shared_ptr<User> User::getUser(std::string username)
{
	std::string errorMessage = "";
	/*if (users.find(username) == users.end())
	{
		errorMessage += ("\nUser with given username" + username + " does not exist!");
		std::cout << errorMessage << std::endl;
		throw std::invalid_argument(errorMessage);
	}*/
	//std::cout << "getuser" << std::endl;
	//std::cout << username << std::endl;
	//std::cout << users.count("Max");
	std::shared_ptr<User> user1 = users.find(username)->second;
	std::cout << users.find(username)->second.get()->username << std::endl;

	//std::cout << "getuser" << std::endl;
	return user1;
}

//int User::getuserType()
//{
//	return this->userType;
//}
//
//std::string  User::getfName()
//{
//	return this->fName;
//}
//
//std::string  User::getlName()
//{
//	return this->lName;
//}
//
//std::string User::getPassword()
//{
//	return this->password;
//}
//
//
//
//void User::setUsertype(int tId)
//{
//	this->userType = tId;
//}
//
//
//void User::setfName(std::string fName)
//{
//	this->fName = fName;
//}
//
//
//void User::setlName(std::string lName)
//{
//	this->lName = lName;
//}
//
//void User::setPassword(std::string password)
//{
//	this->password = password;
//}



#include "user.h"

std::map<std::string, std::shared_ptr<user>> user::users;

user::user() {}


user::user(int utype, std::string username, std::string pw)
{
	
	/*unsigned int tempId = 1;
	std::map<std::string, std::shared_ptr<user>>::iterator it;
	for (it = users.begin(); it != users.end(); it++)
	{
		if (it->first >= tempId)
			tempId++;
	}

	user::userId = tempId;*/
	user::userType = utype;
	user::username = username; 
	setPassword(pw);





	users.insert(std::make_pair(username, std::shared_ptr<user>(this)));

}


user::user(int utype, std::string username, std::string firstName, std::string lastName, std::string pw )
{
	user::userId;  // still missing 
	this->userType = utype; 
	user::username = username; 
	user::fName = firstName; 
	user::lName = lastName; 
	user::password = pw; 
}



user::user(int type, std::string username,  std::string firstName, std::string lastName, std::string pw, int mPhone, std::string city)
{
	user::userType = type;
	user::username = username;
	user::fName = firstName;
	user::lName = lastName;
	user::password = pw;
	user::mPhone = mPhone;
	user::city = city; 
}

user::~user()
{
}

void user::modifyUser(std::string firstName, std::string lastName, int mPhone, std::string city)
{
	if (!firstName.empty()) 
	{
		this->fName = firstName;
	}
	if (!lastName.empty())
	{
		this->lName = lastName;
	}
	if (!mPhone)
	{
		this->mPhone = mPhone;
	}
	if (!city.empty())
	{
		this->city = city;
	}

	return;
}

int user::changePassword(user user, std::string oldPassword, std::string newPassword)
{
	std::string oldPw = user.getPassword();

	if (oldPw == oldPassword)
	{
		user.setPassword(newPassword);
		
		return 1;
	}

	else
	{
		std::cout << "The old password does not match!" << std::endl;
		return 0;
	}
}

std::string user::deleteUser(std::string username)
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

std::string user::deleteUser()
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

std::shared_ptr<user> user::getUser(std::string username)
{
	std::string errorMessage = "";
	if (users.find(username) == users.end())
	{
		errorMessage += ("\nUser with given username" + username + " does not exist!");
		throw std::invalid_argument(errorMessage);
	}
	std::shared_ptr<user> user1 = users.find(username)->second;
	return user1;
}





// get 
	/*unsigned int user::getId()
	{
		return this->userId;
	}*/

	int user::getuserType()
	{
		return this->userType;
	}

	std::string  user::getfName()
	{
		return this->fName;
	}

	std::string  user::getlName()
	{
		return this->lName;
	}

	std::string  user::getPassword()
	{
		return this->password;
	}

	// set 

	/*void user::setId()
	{
        
	}*/

	void user::setUsertype(int tId)
	{ 
		this->userType = tId; 
	}


	void user::setfName(std::string fName)
	{
		this->fName = fName; 
	}


	void user::setlName(std::string lName)
	{
		this->lName = lName; 
	}

	void user::setPassword(std::string password)
	{
		this->password = password;
	}






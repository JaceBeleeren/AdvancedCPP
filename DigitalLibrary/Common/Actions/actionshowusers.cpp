#include "actionshowusers.h"
#include "../User.h"


ActionShowUsers::ActionShowUsers()
{
}


ActionShowUsers::~ActionShowUsers()
{
}


bool ActionShowUsers::parseToStruct(std::shared_ptr<char> newPayload)
{
	payload = newPayload;
	payload_size = 0;	

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionShowUsers::parseToPayload()
{
	unsigned int add;
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionShowUsers::response_parseToStruct(std::shared_ptr<char> newPayload)
{
	response_payload = newPayload;
	response_size = 0;

	User::users.empty();

	if (!check_response_size(response_size))
		return false;
	response_struct.size = Protocol::charToUInt(response_payload.get() + response_size);
	response_size += sizeof(unsigned int);

	std::list<User>::iterator it;
	it = response_struct.list.begin();

	for (unsigned int i = 0; i < response_struct.size; i++)//iterate ofer every book submitted
	{
		//// int userType
		//if (!check_payload_size(payload_size))
		//	return false;
		//int userType = Protocol::charToUInt(payload.get() + payload_size);
		//payload_size += sizeof(int);


		//std::string username
		if (!check_response_size(response_size))
			return false;
		std::string username = std::string(response_payload.get() + response_size);
		response_size += username.size() + sizeof(char);

		//std::string fName
		if (!check_response_size(response_size))
			return false;
		std::string fName = std::string(response_payload.get() + response_size);
		response_size += fName.size() + sizeof(char);

		//std::string lName
		if (!check_response_size(response_size))
			return false;
		std::string lName = std::string(response_payload.get() + response_size);
		response_size += lName.size() + sizeof(char);

		//std::string password
		if (!check_response_size(response_size))
			return false;
		std::string password = std::string(response_payload.get() + response_size);
		response_size += password.size() + sizeof(char);


		//int mPhone
		if (!check_response_size(response_size))
			return false;
		int mPhone = Protocol::charToInt(response_payload.get() + response_size);
		response_size += sizeof(int);

		//std::string city
		if (!check_response_size(response_size))
			return false;
		std::string city = std::string(response_payload.get() + response_size);
		response_size += city.size() + sizeof(char);

		//int  countBorrowedBooks;
		if (!check_response_size(response_size))
			return false;
		int countBorrowedBooks = Protocol::charToInt(response_payload.get() + response_size);
		response_size += sizeof(int);


		try
		{
			

			/*std::shared_ptr<User>*/ User u;
			u.username = username;
			/*u.get()->userType = userType;*/
			u.fName = fName;
			u.lName = lName;
			u.password = password;
			u.mPhone = mPhone; 
			u.city = city; 
			u.countBorrowedBooks = countBorrowedBooks;

			response_struct.list.insert(it,u);
			
		}
		catch (std::exception& e)
		{
			//message printed in book constructor
			//return false;
		}

	}

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionShowUsers::response_parseToPayload()
{
	unsigned int add;
	response_size = 0;
	if (!response_payload)
		response_payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());


	add = sizeof(unsigned int);
	if (!check_response_size(response_size + add))
		return false;
	response_struct.size = User::users.size();
	Protocol::uintToChar(response_struct.size, response_payload.get() + response_size);//4bytes
	response_size += add;



	std::map<std::string, std::shared_ptr<User>>::iterator it;
	for (it = User::users.begin(); it != User::users.end(); it++)
	{
		// int userType

		//add = sizeof(int);
		//if (!check_payload_size(payload_size + add))
		//	return false;
		//Protocol::intToChar(it->second.get()->userType, payload.get() + payload_size);//4bytes
		//payload_size += add;


		// string username

		add = it->second.get()->username.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->second.get()->username.begin(), it->second.get()->username.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;


		//std::string fName
		add = it->second.get()->fName.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->second.get()->fName.begin(), it->second.get()->fName.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;


		//std::string lName
		add = it->second.get()->lName.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->second.get()->lName.begin(), it->second.get()->lName.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//std::string password
		add = it->second.get()->password.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->second.get()->password.begin(), it->second.get()->password.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;


		//int mPhone
		add = sizeof(int);
		if (!check_response_size(response_size + add))
			return false;
		Protocol::intToChar(it->second.get()->mPhone, response_payload.get() + response_size);//4bytes
		response_size += add;

		//std::string city
		add = it->second.get()->city.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->second.get()->city.begin(), it->second.get()->city.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//int  countBorrowedBooks;
		add = sizeof(int);
		if (!check_response_size(response_size + add))
			return false;
		Protocol::intToChar(it->second.get()->countBorrowedBooks, response_payload.get() + response_size);//4bytes
		response_size += add;
	}

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}
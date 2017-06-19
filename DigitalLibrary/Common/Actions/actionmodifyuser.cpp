#include "actionmodifyuser.h"
#include "..\User.h"



ActionModifyUser::ActionModifyUser()
{
}


ActionModifyUser::~ActionModifyUser()
{
}


bool ActionModifyUser::parseToStruct(std::shared_ptr<char> newPayload)
{
	payload = newPayload;
	payload_size = 0;

	//std::string username
	if (!check_payload_size(payload_size))
		return false;
	std::string username = std::string(payload.get() + payload_size);
	payload_size += username.size() + 1;		

	
	//std::string fName
	if (!check_payload_size(payload_size))
		return false;
	std::string fName = std::string(payload.get() + payload_size);
	payload_size += fName.size() + 1;

	//std::string lName
	if (!check_payload_size(payload_size))
		return false;
	std::string lName = std::string(payload.get() + payload_size);
	payload_size += lName.size() + 1;


	// int mPhone
	if (!check_payload_size(payload_size))
		return false;
	unsigned int mPhone = Protocol::charToInt(payload.get() + payload_size);
	payload_size += sizeof(int);

	//std::string city
	if (!check_payload_size(payload_size))
		return false;
	std::string city = std::string(payload.get() + payload_size);
	payload_size += city.size() + 1;

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;


	try
	{
		std::shared_ptr<User> user = User::getUser(username);

		user.get()->username = username;
		user.get()->fName = fName;
		user.get()->lName = lName;
		user.get()->mPhone = mPhone;
		user.get()->city = city;		

		payload_struct.user = user;
		std::cout << "username:" << username << std::endl;
		std::cout << "new fname:" << fName << std::endl;
		std::cout << "new fname:" << user.get()->fName << std::endl;
		std::cout << "new lname:" << lName << std::endl;
		std::cout << "new lname" << user.get()->lName << std::endl;
	}
	catch (std::exception& e)
	{
		//message printed in book constructor
		return false;
	}


	
	std::string errorMessage = payload_struct.user.get()->modifyUser(fName, lName, mPhone, city, username);
	if (errorMessage == "")
	{
		std::cout << errorMessage;
		return false;
	}

	return true;
}

bool ActionModifyUser::parseToPayload()
{
	unsigned int add;
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());


	// std::string username

	add = payload_struct.user.get()->username.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.user.get()->username.begin(), payload_struct.user.get()->username.end(), payload.get() + payload_size);
	payload_size += add;

	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;

	// std::string fName
	add = payload_struct.user.get()->fName.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::cout << "Name" << payload_struct.user.get()->fName; // Getter entfernen
	std::copy(payload_struct.user.get()->fName.begin(), payload_struct.user.get()->fName.end(), payload.get() + payload_size);
	payload_size += add;

	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;


	// std::string lName

	add = payload_struct.user.get()->lName.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::cout << "Name" << payload_struct.user.get()->lName; // Getter entfernen
	std::copy(payload_struct.user.get()->lName.begin(), payload_struct.user.get()->lName.end(), payload.get() + payload_size);
	payload_size += add;


	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;


	//unsigned int mPhone
	add = sizeof(unsigned int);
	if (!check_payload_size(payload_size + add))
		return false;
	Protocol::intToChar(payload_struct.user.get()->mPhone, payload.get() + payload_size);   //4bytes
	payload_size += add;

	// std::string city
	add = payload_struct.user.get()->city.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.user.get()->city.begin(), payload_struct.user.get()->city.end(), payload.get() + payload_size);
	payload_size += add;

	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;


	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionModifyUser::response_parseToStruct(std::shared_ptr<char> newPayload)
{
	response_payload = newPayload;
	response_size = 0;

	//unsigned char success
	if (!check_response_size(response_size))
		return false;
	response_struct.response = *(response_payload.get() + response_size);
	response_size += 1;

	//std::string response
	if (!check_response_size(response_size))
		return false;
	response_struct.response = std::string(response_payload.get() + response_size);
	response_size = response_struct.response.size() + 1;

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionModifyUser::response_parseToPayload()
{
	unsigned int add;
	response_size = 0;
	if (!response_payload)
		response_payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	// unsigned char success
		add = sizeof(unsigned char);
	if (!check_response_size(response_size + add))
		return false;
	response_payload.get()[response_size] = response_struct.success;
	response_size += sizeof(unsigned char);

	//std::string response
	add = response_struct.response.size();
	if (!check_response_size(response_size + add))
		return false;
	std::copy(response_struct.response.begin(), response_struct.response.end(), response_payload.get() + response_size);
	response_size += add;

	add = sizeof(char);
	if (!check_response_size(response_size + add))
		return false;
	response_payload.get()[response_size] = '\0';
	response_size += add;

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}
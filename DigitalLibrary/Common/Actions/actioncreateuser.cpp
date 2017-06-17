
#include "actioncreateuser.h"



ActionCreateUser::ActionCreateUser()
{
}


ActionCreateUser::~ActionCreateUser()
{
}


bool ActionCreateUser::parseToStruct(std::shared_ptr<char> newPayload)
{
	payload = newPayload;
	payload_size = 0;

	
	////unsigned int userId
	//if (!check_payload_size(payload_size))
	//	return false;
	//unsigned int userId = Protocol::charToUInt(payload.get() + payload_size);
	//payload_size += sizeof(unsigned int);

	// int userType
	if (!check_payload_size(payload_size))
		return false;
	int userType = Protocol::charToUInt(payload.get() + payload_size);
	payload_size += sizeof(int);


	//std::string username
	if (!check_payload_size(payload_size))
		return false;
	std::string username = std::string(payload.get() + payload_size);
	payload_size = username.size() + sizeof(char);

	//std::string fName
	if (!check_payload_size(payload_size))
		return false;
	std::string fName = std::string(payload.get() + payload_size);
	payload_size = fName.size() + sizeof(char);

	//std::string lName
	if (!check_payload_size(payload_size))
		return false;
	std::string lName = std::string(payload.get() + payload_size);
	payload_size = lName.size() + sizeof(char);

	//std::string password
	if (!check_payload_size(payload_size))
		return false;
	std::string password = std::string(payload.get() + payload_size);
	payload_size = password.size() + sizeof(char);
	

	//int mPhone
	if (!check_payload_size(payload_size))
		return false;
	int mPhone = Protocol::charToInt(payload.get() + payload_size);
	payload_size += sizeof(int);

	//std::string city
	if (!check_payload_size(payload_size))
		return false;
	std::string city = std::string(payload.get() + payload_size);
	payload_size = city.size() + sizeof(char);

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;

	try
	{
		/*payload_struct.user.get()->username = username; 
		payload_struct.user.get()->password = password; 
		payload_struct.user.get()->userType = userType;
		payload_struct.user.get()->fName = fName; 
		payload_struct.user.get()->lName = lName; 
		payload_struct.user.get()->mPhone = mPhone; 
		payload_struct.user.get()->city = city; */
	}
	catch (std::exception& e)
	{
		return false;
	}

	return true;
}

bool ActionCreateUser::parseToPayload()
{
	unsigned int add;
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	
	//// unsigned int userId
	//add = sizeof(unsigned int);
	//if (!check_payload_size(payload_size + add))
	//	return false;
	//Protocol::uintToChar(payload_struct.user.get()->getId(), payload.get() + payload_size);//4bytes
	//payload_size += add;

	// int userType
	
	add = sizeof(int);
	if (!check_payload_size(payload_size + add))
		return false;
	Protocol::intToChar(payload_struct.user.get()->userType, payload.get() + payload_size);//4bytes
	payload_size += add;
	

	// string username

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


	////std::string fName
	//add = payload_struct.user.get()->getfName().size();
	//if (!check_payload_size(payload_size + add))
	//	return false;
	//std::copy(payload_struct.user.get()->getfName().begin(), payload_struct.user.get()->getfName().end(), payload.get() + payload_size);
	//payload_size += add;

	//add = sizeof(char);
	//if (!check_payload_size(payload_size + add))
	//	return false;
	//payload.get()[payload_size] = '\0';
	//payload_size += add;

	//
	////std::string lName
	//add = payload_struct.user.get()->getlName().size();
	//if (!check_payload_size(payload_size + add))
	//return false;
	//std::copy(payload_struct.user.get()->getlName().begin(), payload_struct.user.get()->getlName().end(), payload.get() + payload_size);
	//payload_size += add;

	//add = sizeof(char);
	//if (!check_payload_size(payload_size + add))
	//return false;
	//payload.get()[payload_size] = '\0';
	//payload_size += add;
	//

	////int mPhone
	//add = sizeof(int);
	//if (!check_payload_size(payload_size + add))
	//	return false;
	//Protocol::intToChar(payload_struct.user.get()->mPhone, payload.get() + payload_size);//4bytes
	//payload_size += add;

	////std::string city
	//add = payload_struct.user.get()->city.size();
	//if (!check_payload_size(payload_size + add))
	//	return false;
	//std::copy(payload_struct.user.get()->city.begin(), payload_struct.user.get()->city.end(), payload.get() + payload_size);
	//payload_size += add;

	//add = sizeof(char);
	//if (!check_payload_size(payload_size + add))
	//	return false;
	//payload.get()[payload_size] = '\0';
	//payload_size += add;
	//

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;

	

	return true;
}

bool ActionCreateUser::response_parseToStruct(std::shared_ptr<char> newPayload)
{
	response_payload = newPayload;
	response_size = 0;

	//unsigned int id
	if (!check_response_size(response_size))
		return false;
	response_struct.id = Protocol::charToUInt(response_payload.get() + response_size);
	response_size += sizeof(unsigned int);

	//std::string response
	if (!check_response_size(response_size))
		return false;
	response_struct.response = std::string(response_payload.get() + response_size);
	response_size = response_struct.response.size() + sizeof(char);

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionCreateUser::response_parseToPayload()
{
	unsigned int add;
	response_size = 0;
	if (!response_payload)
		response_payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	//unsigned int id
	add = sizeof(unsigned int);
	if (!check_response_size(response_size + add))
		return false;
	Protocol::uintToChar(response_struct.id, response_payload.get() + response_size);//4bytes
	response_size += add;

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
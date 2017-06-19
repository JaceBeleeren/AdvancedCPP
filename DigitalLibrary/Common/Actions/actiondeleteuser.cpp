#include "actiondeleteuser.h"



ActionDeleteUser::ActionDeleteUser()
{
}


ActionDeleteUser::~ActionDeleteUser()
{
}


bool ActionDeleteUser::parseToStruct(std::shared_ptr<char> newPayload)
{
	payload = newPayload;
	payload_size = 0;

	if (!check_payload_size(payload_size))
		return false;
	std::string username = std::string(payload.get() + payload_size);
	payload_size += username.size() + sizeof(char);


	try
	{
		/*std::map < std::string, std::shared_ptr<User>>::iterator it; 
		for (it = payload_struct.user.get()->users.begin(); it != payload_struct.user.get()->users.end(); it++)
		{*/
			payload_struct.user = payload_struct.user.get()->getUser(username);
			/*if (it->second.get()->username == username)
			{
				 = it->second;
			}*/
		/*}*/
		/*payload_struct.user = payload_struct.user.get()->users.find(username)->second;*/
	}

	catch (std::exception& e)
	{
		return false;
	}


	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionDeleteUser::parseToPayload()
{
	unsigned int add;
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	add = payload_struct.user.get()->username.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.user.get()->username.begin(), payload_struct.user.get()->username.end(), payload.get() + payload_size);
	payload_size += add;

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionDeleteUser::response_parseToStruct(std::shared_ptr<char> newPayload)
{
	response_payload = newPayload;
	response_size = 0;

	//unsigned char success
	if (!check_response_size(response_size))
		return false;
	response_struct.success = *(response_payload.get() + response_size);
	response_size += sizeof(unsigned char);

	//std::string response
	if (!check_response_size(response_size))
		return false;
	response_struct.response = std::string(response_payload.get() + response_size);
	response_size = response_struct.response.size() + sizeof(char);

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionDeleteUser::response_parseToPayload()
{
	unsigned int add;
	response_size = 0;
	if (!response_payload)
		response_payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	//unsigned char success
	add = sizeof(unsigned char);
	if (!check_response_size(response_size + add))
		return false;
	response_payload.get()[response_size] = response_struct.success;
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
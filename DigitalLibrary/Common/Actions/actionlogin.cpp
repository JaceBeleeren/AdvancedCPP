#include "actionlogin.h"



ActionLogin::ActionLogin()
{
}


ActionLogin::~ActionLogin()
{
}

bool ActionLogin::parseToStruct(std::shared_ptr<char> newPayload)
{
	payload = newPayload;
	payload_size = 0;

	//std::string user
	if (!check_payload_size(payload_size))
		return false;
	payload_struct.user = std::string(payload.get() + payload_size);
		payload_size = payload_struct.user.size() + sizeof(char);

	//std::string password
	if (!check_payload_size(payload_size))
		return false;
	payload_struct.password = std::string(payload.get() + payload_size);
		payload_size += payload_struct.password.size() + sizeof(char);
	
	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionLogin::parseToPayload()
{
	unsigned int add;
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	//std::string user
	add = payload_struct.user.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.user.begin(), payload_struct.user.end(), payload.get());
	payload_size += add;

	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;

	//std::string password
	add = payload_struct.password.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.password.begin(), payload_struct.password.end(), payload.get() + payload_size);
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

bool ActionLogin::response_parseToStruct(std::shared_ptr<char> newPayload)
{
	response_payload = newPayload;
	response_size = 0;

	//unsigned int id
	if (!check_response_size(response_size))
		return false;
	response_struct.id = Protocol::charToUInt(payload.get() + response_size);
	response_size += sizeof(unsigned int);

	//unsigned int permissions
	if (!check_response_size(response_size))
		return false;
	response_struct.permissions = Protocol::charToUInt(payload.get() + response_size);
	response_size += sizeof(unsigned int);

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionLogin::response_parseToPayload()
{
	unsigned int add;
	response_size = 0;
	if (!response_payload)
		response_payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	//unsigned int id
	add = sizeof(unsigned int);
	if (!check_response_size(response_size + add))
		return false;
	Protocol::uintToChar(response_struct.id, payload.get() + response_size);//4bytes
	response_size += add;

	//unsigned int permissions
	add = sizeof(unsigned int);
	if (!check_response_size(response_size + add))
		return false;
	Protocol::uintToChar(response_struct.permissions, payload.get() + response_size);//4bytes
	response_size += add;

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}


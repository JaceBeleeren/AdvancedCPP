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
	payload_struct.user = std::string(payload.get());
		payload_size = payload_struct.user.size() + 1;

	//std::string password
	payload_struct.password = std::string(payload.get() + payload_size);
		payload_size += payload_struct.password.size() + 1;
	
	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionLogin::parseToPayload()
{
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	//std::string user
	std::copy(payload_struct.user.begin(), payload_struct.user.end(), payload.get());
		payload_size += payload_struct.user.size();
		payload.get()[payload_size] = '\0';
			payload_size += 1;

	//std::string password
	std::copy(payload_struct.password.begin(), payload_struct.password.end(), payload.get() + payload_size);
		payload_size += payload_struct.password.size();
		payload.get()[payload_size] = '\0';
			payload_size += 1;

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionLogin::response_parseToStruct(std::shared_ptr<char> newPayload)
{
	response_payload = newPayload;
	response_size = 0;

	//unsigned int id
	response_struct.id = Protocol::charToUInt(payload.get() + response_size);
	response_size += sizeof(unsigned int);

	//unsigned int permissions
	response_struct.permissions = Protocol::charToUInt(payload.get() + response_size);
	response_size += sizeof(unsigned int);
	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionLogin::response_parseToPayload()
{
	response_size = 0;
	if (!response_payload)
		response_payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	//unsigned int id
	Protocol::uintToChar(response_struct.id, payload.get() + response_size);//4bytes
	response_size += sizeof(unsigned int);

	//unsigned int permissions
	Protocol::uintToChar(response_struct.permissions, payload.get() + response_size);//4bytes
	response_size += sizeof(unsigned int);

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}


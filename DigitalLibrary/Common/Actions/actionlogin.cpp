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
	payload_struct.user = std::string(payload.get());
		payload_size = payload_struct.user.size() + 1;
	payload_struct.test = Protocol::charToUInt(payload.get() + payload_size);
		payload_size += sizeof(unsigned int);
	payload_struct.string = std::string(payload.get() + payload_size);
		payload_size += payload_struct.string.size() + 1;
	return true;
}

bool ActionLogin::parseToPayload()
{
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	std::copy(payload_struct.user.begin(), payload_struct.user.end(), payload.get());
		payload_size += payload_struct.user.size();
		payload.get()[payload_size] = '\0';
			payload_size += 1;
	Protocol::uintToChar(payload_struct.test, payload.get() + payload_size);//4bytes
		payload_size += sizeof(unsigned int);
	std::copy(payload_struct.string.begin(), payload_struct.string.end(), payload.get() + payload_size);
		payload_size += payload_struct.string.size();
		payload.get()[payload_size] = '\0';
			payload_size += 1;
	return true;
}

bool ActionLogin::response_parseToStruct(std::shared_ptr<char> datagram)
{
	return false;
}

bool ActionLogin::response_parseToPayload()
{
	return false;
}


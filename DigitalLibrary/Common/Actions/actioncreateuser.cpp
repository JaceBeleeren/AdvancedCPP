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

	/*
	//std::string
	if (!check_payload_size(payload_size))
		return false;
	payload_struct.string = std::string(payload.get() + payload_size);
	payload_size = payload_struct.string.size() + sizeof(char);

	//unsigned int
	if (!check_payload_size(payload_size))
		return false;
	payload_struct.uint = Protocol::charToUInt(payload.get() + payload_size);
	payload_size += sizeof(unsigned int);

	//int
	if (!check_payload_size(payload_size))
		return false;
	payload_struct.int = Protocol::charToInt(payload.get() + payload_size);
	payload_size += sizeof(int);

	//char/unsigned char
	if (!check_payload_size(payload_size))
		return false;
	payload_struct.char = *(payload.get() + payload_size);
	payload_size += sizeof(char);
	*/

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionCreateUser::parseToPayload()
{
	unsigned int add;
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	/*
	//std::string
	add = payload_struct.string.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.string.begin(), payload_struct.string.end(), payload.get() + payload_size);
	payload_size += add;

	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;

	//unsigned int
	add = sizeof(unsigned int);
	if (!check_payload_size(payload_size + add))
		return false;
	Protocol::uintToChar(payload_struct.uint, payload.get() + payload_size);//4bytes
	payload_size += add;

	//int
	add = sizeof(int);
	if (!check_payload_size(payload_size + add))
		return false;
	Protocol::intToChar(payload_struct.int, payload.get() + payload_size);//4bytes
	payload_size += add;

	//char/unsigned char
	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = payload_struct.char;
	size += add;
	*/

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionCreateUser::response_parseToStruct(std::shared_ptr<char> newPayload)
{
	response_payload = newPayload;
	response_size = 0;

	/*
	//std::string
	if (!check_response_size(response_size))
		return false;
	response_struct.string = std::string(response_payload.get() + response_size);
	response_size = response_struct.string.size() + sizeof(char);

	//unsigned int
	if (!check_response_size(response_size))
		return false;
	response_struct.uint = Protocol::charToUInt(response_payload.get() + response_size);
	response_size += sizeof(unsigned int);

	//int
	if (!check_response_size(response_size))
		return false;
	response_struct.int = Protocol::charToUInt(response_payload.get() + response_size);
	response_size += sizeof(unsigned int);

	//char/unsigned char
	if (!check_response_size(response_size))
		return false;
	response_struct.char = *(response_payload.get() + response_size);
	response_size += sizeof(char);
	*/

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionCreateUser::response_parseToPayload()
{
	response_size = 0;
	if (!response_payload)
		response_payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	/*
	//std::string
	add = response_struct.string.size();
	if (!check_response_size(response_size + add))
		return false;
	std::copy(response_struct.string.begin(), response_struct.string.end(), response_payload.get() + response_size);
	response_size += add;

	add = sizeof(char);
	if (!check_response_size(response_size + add))
		return false;
	response_payload.get()[response_size] = '\0';
	response_size += add;

	//unsigned int
	add = sizeof(unsigned int);
	if (!check_response_size(response_size + add))
		return false;
	Protocol::uintToChar(response_struct.uint, response_payload.get() + response_size);//4bytes
	response_size += add;

	//int
	add = sizeof(int);
	if (!check_response_size(response_size + add))
		return false;
	Protocol::uintToChar(response_struct.int, response_payload.get() + response_size);//4bytes
	response_size += add;

	//char/unsigned char
	add = sizeof(char);
	if (!check_response_size(response_size + add))
		return false;
	response_payload.get()[response_size] = response_struct.char;
	response_size += add;
	*/

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}
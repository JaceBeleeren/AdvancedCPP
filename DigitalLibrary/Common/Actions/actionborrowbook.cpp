#include "actionborrowbook.h"



ActionBorrowBook::ActionBorrowBook()
{
}


ActionBorrowBook::~ActionBorrowBook()
{
}

bool ActionBorrowBook::parseToStruct(std::shared_ptr<char> newPayload)
{
	payload = newPayload;
	payload_size = 0;

	//int id
	if (!check_payload_size(payload_size))
		return false;
	unsigned int id = Protocol::charToInt(payload.get() + payload_size);
	payload_size += sizeof(unsigned int);

	//std::string username
	if (!check_payload_size(payload_size))
		return false;
	std::string username = std::string(payload.get() + payload_size);
	payload_size += username.size() + 1;

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionBorrowBook::parseToPayload()
{
	unsigned int add;
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	//unsigned int id
	add = sizeof(unsigned int);
	if (!check_payload_size(payload_size + add))
		return false;
	Protocol::uintToChar(payload_struct.id, payload.get() + payload_size);//4bytes
	payload_size += add;

	// std:: string username
	add = payload_struct.username.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.username.begin(), payload_struct.username.end(), payload.get() + payload_size);
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

bool ActionBorrowBook::response_parseToStruct(std::shared_ptr<char> newPayload)
{
	response_payload = newPayload;
	response_size = 0;

	//unsigned char success
	if (!check_response_size(response_size))
		return false;
	response_struct.response = *(response_payload.get() + response_size);
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

bool ActionBorrowBook::response_parseToPayload()
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
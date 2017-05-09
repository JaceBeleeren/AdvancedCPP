#include "actionaddbook.h"



ActionAddBook::ActionAddBook()
{
}


ActionAddBook::~ActionAddBook()
{
}


bool ActionAddBook::parseToStruct(std::shared_ptr<char> newPayload)
{
	payload = newPayload;
	payload_size = 0;

	//std::string title
	payload_struct.title = std::string(payload.get());
	payload_size = payload_struct.title.size() + 1;

	//std::string author
	payload_struct.author = std::string(payload.get() + payload_size);
	payload_size += payload_struct.author.size() + 1;

	//std::string summary
	payload_struct.summary = std::string(payload.get() + payload_size);
	payload_size += payload_struct.summary.size() + 1;

	//std::string publisher
	payload_struct.publisher = std::string(payload.get() + payload_size);
	payload_size += payload_struct.publisher.size() + 1;

	//unsigned int year
	payload_struct.year = Protocol::charToUInt(payload.get() + payload_size);
	payload_size += sizeof(unsigned int);

	//std::string isbn
	payload_struct.isbn = std::string(payload.get() + payload_size);
	payload_size += payload_struct.isbn.size() + 1;

	//unsigned int amount
	payload_struct.amount = Protocol::charToUInt(payload.get() + payload_size);
	payload_size += sizeof(unsigned int);

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionAddBook::parseToPayload()
{
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	//std::string title
	std::copy(payload_struct.title.begin(), payload_struct.title.end(), payload.get());
	payload_size += payload_struct.title.size();
	payload.get()[payload_size] = '\0';
	payload_size += 1;

	//std::string author
	std::copy(payload_struct.author.begin(), payload_struct.author.end(), payload.get() + payload_size);
	payload_size += payload_struct.author.size();
	payload.get()[payload_size] = '\0';
	payload_size += 1;

	//std::string summary
	std::copy(payload_struct.summary.begin(), payload_struct.summary.end(), payload.get() + payload_size);
	payload_size += payload_struct.summary.size();
	payload.get()[payload_size] = '\0';
	payload_size += 1;

	//std::string publisher
	std::copy(payload_struct.publisher.begin(), payload_struct.publisher.end(), payload.get() + payload_size);
	payload_size += payload_struct.publisher.size();
	payload.get()[payload_size] = '\0';
	payload_size += 1;

	//unsigned int year
	Protocol::uintToChar(payload_struct.year, payload.get() + payload_size);//4bytes
	payload_size += sizeof(unsigned int);

	//std::string isbn
	std::copy(payload_struct.isbn.begin(), payload_struct.isbn.end(), payload.get() + payload_size);
	payload_size += payload_struct.isbn.size();
	payload.get()[payload_size] = '\0';
	payload_size += 1;

	//unsigned int amount
	Protocol::uintToChar(payload_struct.amount, payload.get() + payload_size);//4bytes
	payload_size += sizeof(unsigned int);

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionAddBook::response_parseToStruct(std::shared_ptr<char> newPayload)
{
	response_payload = newPayload;
	response_size = 0;
	//unsigned int id
	response_struct.id = Protocol::charToUInt(response_payload.get() + response_size);
	response_size += sizeof(unsigned int);

	//std::string response
	response_struct.response = std::string(response_payload.get() + response_size);
	response_size = response_struct.response.size() + 1;

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionAddBook::response_parseToPayload()
{
	response_size = 0;
	if (!response_payload)
		response_payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	//unsigned int id
	Protocol::uintToChar(response_struct.id, response_payload.get() + response_size);//4bytes
	response_size += sizeof(unsigned int);

	//std::string response
	std::copy(response_struct.response.begin(), response_struct.response.end(), response_payload.get() + response_size);
	response_size += response_struct.response.size();
	response_payload.get()[response_size] = '\0';
	response_size += 1;

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}
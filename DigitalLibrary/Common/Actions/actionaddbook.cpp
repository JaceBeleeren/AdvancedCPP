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
	if (!check_payload_size(payload_size))
		return false;
	std::string title = std::string(payload.get() + payload_size);
	payload_size = title.size() + sizeof(char);

	//std::string author
	if (!check_payload_size(payload_size))
		return false;
	std::string author = std::string(payload.get() + payload_size);
	payload_size += author.size() + sizeof(char);

	//std::string summary
	if (!check_payload_size(payload_size))
		return false;
	std::string summary = std::string(payload.get() + payload_size);
	payload_size += summary.size() + sizeof(char);

	//std::string publisher
	if (!check_payload_size(payload_size))
		return false;
	std::string publisher = std::string(payload.get() + payload_size);
	payload_size += publisher.size() + sizeof(char);

	//unsigned int year
	if (!check_payload_size(payload_size))
		return false;
	unsigned int year = Protocol::charToUInt(payload.get() + payload_size);
	payload_size += sizeof(unsigned int);

	//std::string isbn
	if (!check_payload_size(payload_size))
		return false;
	std::string isbn = std::string(payload.get() + payload_size);
	payload_size += isbn.size() + sizeof(char);

	//unsigned int amount
	if (!check_payload_size(payload_size))
		return false;
	unsigned int amount = Protocol::charToUInt(payload.get() + payload_size);
	payload_size += sizeof(unsigned int);

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	try
	{
		std::shared_ptr<Book> book = std::shared_ptr<Book>(new Book());
		book.get()->constructorBook(title, author, summary, publisher, year, isbn, amount);
		payload_struct.book = book;
	}
	catch (std::exception& e)
	{
		//message printed in book constructor
		return false;
	}
	return true;
}

bool ActionAddBook::parseToPayload()
{
	unsigned int add;
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	//std::string title
	add = payload_struct.book.get()->title.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.book.get()->title.begin(), payload_struct.book.get()->title.end(), payload.get() + payload_size);
	payload_size += add;

	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;

	//std::string author
	add = payload_struct.book.get()->author.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.book.get()->author.begin(), payload_struct.book.get()->author.end(), payload.get() + payload_size);
	payload_size += add;

	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;

	//std::string summary
	add = payload_struct.book.get()->summary.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.book.get()->summary.begin(), payload_struct.book.get()->summary.end(), payload.get() + payload_size);
	payload_size += add;

	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;

	//std::string publisher
	add = payload_struct.book.get()->publisher.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.book.get()->publisher.begin(), payload_struct.book.get()->publisher.end(), payload.get() + payload_size);
	payload_size += add;

	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;

	//unsigned int year
	add = sizeof(unsigned int);
	if (!check_payload_size(payload_size + add))
		return false;
	Protocol::uintToChar(payload_struct.book.get()->year, payload.get() + payload_size);//4bytes
	payload_size += add;

	//std::string isbn
	add = payload_struct.book.get()->getIsbn().size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.book.get()->getIsbn().begin(), payload_struct.book.get()->getIsbn().end(), payload.get() + payload_size);
	payload_size += add;

	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;

	//unsigned int amount
	add = sizeof(unsigned int);
	if (!check_payload_size(payload_size + add))
		return false;
	Protocol::uintToChar(payload_struct.book.get()->getAmount(), payload.get() + payload_size);//4bytes
	payload_size += add;

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionAddBook::response_parseToStruct(std::shared_ptr<char> newPayload)
{
	response_payload = newPayload;
	response_size = 0;

	//unsigned char success
	if (!check_response_size(response_size))
		return false;
	response_struct.success = *(response_payload.get() + response_size);
	response_size += sizeof(unsigned char);

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

bool ActionAddBook::response_parseToPayload()
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
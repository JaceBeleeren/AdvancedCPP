#include "actionshowbooks.h"



ActionShowBooks::ActionShowBooks()
{
}


ActionShowBooks::~ActionShowBooks()
{
}


bool ActionShowBooks::parseToStruct(std::shared_ptr<char> newPayload)
{
	payload = newPayload;
	payload_size = 0;

	/*
	nothing to be done here
	*/

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionShowBooks::parseToPayload()
{
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	/*
	nothing to be done here
	*/

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionShowBooks::response_parseToStruct(std::shared_ptr<char> newPayload)
{
	response_payload = newPayload;
	response_size = 0;

	Book::books.empty();
	//unsigned int size
	if (!check_response_size(response_size))
		return false;
	response_struct.size = Protocol::charToUInt(response_payload.get() + response_size);
	response_size += sizeof(unsigned int);
	for (unsigned int i = 0; i < response_struct.size; i++)//iterate ofer every book submitted
	{
		//std::string title
		if (!check_response_size(response_size))
			return false;
		std::string title = std::string(response_payload.get() + response_size);
		response_size = title.size() + sizeof(char);

		//std::string author
		if (!check_response_size(response_size))
			return false;
		std::string author = std::string(response_payload.get() + response_size);
		response_size += author.size() + sizeof(char);

		//std::string summary
		if (!check_response_size(response_size))
			return false;
		std::string summary = std::string(response_payload.get() + response_size);
		response_size += summary.size() + sizeof(char);

		//std::string publisher
		if (!check_response_size(response_size))
			return false;
		std::string publisher = std::string(response_payload.get() + response_size);
		response_size += publisher.size() + sizeof(char);

		//unsigned int year
		if (!check_response_size(response_size))
			return false;
		unsigned int year = Protocol::charToUInt(response_payload.get() + response_size);
		response_size += sizeof(unsigned int);

		//std::string isbn
		if (!check_response_size(response_size))
			return false;
		std::string isbn = std::string(response_payload.get() + response_size);
		response_size += isbn.size() + sizeof(char);

		//unsigned int amount
		if (!check_response_size(response_size))
			return false;
		unsigned int amount = Protocol::charToUInt(response_payload.get() + response_size);
		response_size += sizeof(unsigned int);

		try
		{
			std::shared_ptr<Book> book = std::shared_ptr<Book>(new Book());
			book.get()->constructorBook(title, author, summary, publisher, year, isbn, amount);
		}
		catch (std::exception& e)
		{
			//message printed in book constructor
			//return false;
		}
	}

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionShowBooks::response_parseToPayload()
{
	unsigned int add;
	response_size = 0;
	if (!response_payload)
		response_payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	//unsigned int size
	add = sizeof(unsigned int);
	if (!check_response_size(response_size + add))
		return false;
	response_struct.size = Book::books.size();
	Protocol::uintToChar(response_struct.size, response_payload.get() + response_size);//4bytes
	response_size += add;

	std::map<unsigned int, std::shared_ptr<Book>>::iterator it;
	for (it = Book::books.begin(); it != Book::books.end(); it++)
	{
		//unsigned int id
		add = sizeof(unsigned int);
		if (!check_response_size(response_size + add))
			return false;
		Protocol::uintToChar(it->second.get()->getId(), response_payload.get() + response_size);//4bytes
		response_size += add;

		//std::string title
		add = it->second.get()->title.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->second.get()->title.begin(), it->second.get()->title.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//std::string author
		add = it->second.get()->author.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->second.get()->author.begin(), it->second.get()->author.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//std::string summary
		add = it->second.get()->summary.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->second.get()->summary.begin(), it->second.get()->summary.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//std::string publisher
		add = it->second.get()->publisher.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->second.get()->publisher.begin(), it->second.get()->publisher.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//unsigned int year
		add = sizeof(unsigned int);
		if (!check_response_size(response_size + add))
			return false;
		Protocol::uintToChar(it->second.get()->year, response_payload.get() + response_size);//4bytes
		response_size += add;

		//std::string isbn
		add = it->second.get()->getIsbn().size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->second.get()->getIsbn().begin(), it->second.get()->getIsbn().end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//unsigned int amount
		add = sizeof(unsigned int);
		if (!check_response_size(response_size + add))
			return false;
		Protocol::uintToChar(it->second.get()->getAmount(), response_payload.get() + response_size);//4bytes
		response_size += add;
	}

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}
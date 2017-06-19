#include "actionsearchbook.h"
#include "..\book.h"
#include <list>



ActionSearchBook::ActionSearchBook()
{
}


ActionSearchBook::~ActionSearchBook()
{
}


bool  ActionSearchBook::parseToStruct(std::shared_ptr<char> newPayload)
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

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;

	try
	{
		
		payload_struct.title = title;
		payload_struct.author = author;
		payload_struct.publisher =publisher;
		payload_struct.year = year;
		
	}
	catch (std::exception& e)
	{
		//message printed in book constructor
		return false;
	}


	return true;
}

bool  ActionSearchBook::parseToPayload()
{
	unsigned int add;
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	//std::string title
	add = payload_struct.title.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.title.begin(), payload_struct.title.end(), payload.get() + payload_size);
	payload_size += add;

	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;

	//std::string author
	add = payload_struct.author.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.author.begin(), payload_struct.author.end(), payload.get() + payload_size);
	payload_size += add;

	add = sizeof(char);
	if (!check_payload_size(payload_size + add))
		return false;
	payload.get()[payload_size] = '\0';
	payload_size += add;


	//std::string publisher
	add = payload_struct.publisher.size();
	if (!check_payload_size(payload_size + add))
		return false;
	std::copy(payload_struct.publisher.begin(), payload_struct.publisher.end(), payload.get() + payload_size);
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
	Protocol::uintToChar(payload_struct.year, payload.get() + payload_size);//4bytes
	payload_size += add;


	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool  ActionSearchBook::response_parseToStruct(std::shared_ptr<char> newPayload)
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
			/*std::shared_ptr<Book> book = std::shared_ptr<Book>(new Book());
			book.get()->constructorBook(title, author, summary, publisher, year, isbn, amount);*/

			Book book; 
			book.constructorBook(title, author, summary, publisher, year, isbn, amount);

			std::list<Book>::iterator it; 
			response_struct.listBooks.insert(it, book);

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

bool ActionSearchBook::response_parseToPayload()
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
	std::string isbn; 

	
		static std::list <std::shared_ptr<Book>>::iterator it;
		static std::list <std::shared_ptr<Book>> list = Book::getBooks(payload_struct.title);
		for (it = list.begin(); it != list.end(); it++)
		{
		//unsigned int id
		add = sizeof(unsigned int);
		if (!check_response_size(response_size + add))
			return false;
		Protocol::uintToChar(it->get()->getId(), response_payload.get() + response_size);//4bytes
		response_size += add;

		//std::string title
		add = it->get()->title.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->get()->title.begin(), it->get()->title.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//std::string author
		add = it->get()->author.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->get()->author.begin(), it->get()->author.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//std::string summary
		add = it->get()->summary.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->get()->summary.begin(), it->get()->summary.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//std::string publisher
		add = it->get()->publisher.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->get()->publisher.begin(), it->get()->publisher.end(), response_payload.get() + response_size);
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
		Protocol::uintToChar(it->get()->year, response_payload.get() + response_size);//4bytes
		response_size += add;

		//std::string isbn
		add = it->get()->getIsbn().size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(it->get()->getIsbn().begin(), it->get()->getIsbn().end(), response_payload.get() + response_size);
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
		Protocol::uintToChar(it->get()->getAmount(), response_payload.get() + response_size);//4bytes
		response_size += add;
		}
	

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}
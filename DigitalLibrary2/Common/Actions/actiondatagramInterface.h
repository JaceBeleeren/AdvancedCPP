#pragma once

#include <memory>
#include <exception>
#include <string>
#include "..\protocol.h"

class ActionDatagramInterface
{
public:
	ActionDatagramInterface() {};
	~ActionDatagramInterface() {};

	//ACTION
	unsigned int action;
	struct PayloadStruct
	{
		//fill with needed data
		//can also be objects (beware of correct handling while parsing!)
	};
	std::shared_ptr<char> payload;
	unsigned int payload_size = 0;
	unsigned int payload_following = 0;
	unsigned char payload_action;
	virtual bool parseToStruct(std::shared_ptr<char> newPayload) = 0;
	/*	
	{
		payload = newPayload;
		payload_size = 0;

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

		if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
			return false;
		return true;
	};*/
	virtual bool parseToPayload() = 0;
	/*
	{
		unsigned int add;
		payload_size = 0;
		if (!payload)
			payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

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
	

		if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
			return false;
		return true;
	};*/

	//Response
	static const unsigned int action_response;
	struct ResponseStruct
	{
	};
	std::shared_ptr<char> response_payload;
	unsigned int response_size = 0;
	unsigned int response_following = 0;
	unsigned char response_action;
	virtual bool response_parseToStruct(std::shared_ptr<char> newPayload) = 0;
	/*
	{
		response_payload = newPayload;
		response_size = 0;

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

		if (response_size > Protocol::MAX_PAYLOAD_SIZE)
			return false;
		return true;
	};*/

	virtual bool response_parseToPayload() = 0;
	/*
	{
		unsigned int add;
		response_size = 0;
		if (!response_payload)
			response_payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

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

		if (response_size > Protocol::MAX_PAYLOAD_SIZE)
			return false;
		return true;
	};*/

protected:
	bool check_response_size(unsigned int size)
	{
		return check_payload_size(size);
	}
	bool check_payload_size(unsigned int size)
	{
		if (size > Protocol::MAX_PAYLOAD_SIZE)
		{
			return false;
			std::cout << "\nPayload Size " << std::to_string(size) << " would exceed maximum allowed size of " << std::to_string(Protocol::MAX_PAYLOAD_SIZE) << " !" << std::endl;
		}
		return true;
	}
};

/*
Examples:
Sending Data:

	//create shared pointer of char array for payload
	std::shared_ptr<char> payload = std::shared_ptr<char>(new char[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());//payload including header
	std::shared_ptr<char> action_payload;//payload excluding header

	unsigned char action;
	unsigned char following;
	unsigned int payload_size;

	if (choosen_action == 0)//some dummy to choose specific action according to user input
	{
		std::shared_ptr<Book> book = std::shared_ptr<Book>(new Book("How to C++", "Ich", "Isn't a title enough?", "FH-Kiel", 2017, "666-666-666", 10));

		ActionAddBook addBook;//create action object
		addBook.payload_struct.book = book; //fill action object with nessesary data
		addBook.parseToPayload();//parse data of actionobject to payload
		action_payload = addBook.payload;//get payload of action object
		payload_size = addBook.payload_size;//get payload size of action object
		action = addBook.action;//get action from action object
		following = 0;
	}
		
	Protocol::encode_header(payload_size, action, following, payload);//encode header and save to payload
	memcpy(payload.get() + Protocol::HEADER_SIZE, action_payload.get(), payload_size);//save action object payload to payload
	payload.get()[Protocol::HEADER_SIZE + payload_size] = '\0';//finish payload with guaranteed 0 terminator
	
	//send payload
	boost::asio::async_write
	(
		socket,
		boost::asio::buffer(payload.get(), Protocol::HEADER_SIZE + payload_size),
		boost::bind(&TCP_Client::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, action)
	);

Receiving Data:	
	void TCP_Connection::handle_read_payload(const boost::system::error_code& error, std::size_t n, std::shared_ptr<char> payload, unsigned int payload_size, unsigned char action, unsigned char following)
	{
	std::cout << "Read payload: " << action << std::endl;
	if (!error)
	{
		if (action == Protocol::ACTION_ADD_BOOK)
		{
			ActionAddBook addBook;//create action object
			addBook.parseToStruct(payload);//get data from payload
			//read data
			std::cout << "Add Book" << std::endl;
			std::cout << "Title: " << addBook.payload_struct.book.get()->title << std::endl;
			std::cout << "Author: " << addBook.payload_struct.book.get()->author << std::endl;
			std::cout << "Summary: " << addBook.payload_struct.book.get()->summary << std::endl;
			std::cout << "Year: " << addBook.payload_struct.book.get()->year << std::endl;
			std::cout << "ISBN: " << addBook.payload_struct.book.get()->getIsbn() << std::endl;
			std::cout << "Amount: " << addBook.payload_struct.book.get()->getAmount() << std::endl;
	}
*/

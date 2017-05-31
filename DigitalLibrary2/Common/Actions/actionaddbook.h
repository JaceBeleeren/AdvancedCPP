#pragma once
#include "actiondatagramInterface.h"
#include "../book.h"
class ActionAddBook :
	public ActionDatagramInterface
{
public:
	ActionAddBook();
	~ActionAddBook();

	//ACTION
	static const unsigned int action = Protocol::ACTION_ADD_BOOK;
	struct PayloadStruct
	{
		std::shared_ptr<Book> book;
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);//will also add Book if possible
	bool parseToPayload();

	//Response
	static const unsigned int action_response = Protocol::ACTION_ADD_BOOK_RESPONSE;
	struct ResponseStruct
	{
		unsigned char success;
		unsigned int id; //new or existing id
		std::string response; //can used to tell that book exists already for example
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


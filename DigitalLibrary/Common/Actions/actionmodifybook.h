#pragma once
#include "actiondatagramInterface.h"
#include "../book.h"
class ActionModifyBook :
	public ActionDatagramInterface
{
public:
	ActionModifyBook();
	~ActionModifyBook();

	//ACTION
	static const unsigned int action = Protocol::ACTION_MODIFY_BOOK;
	struct PayloadStruct
	{
		std::shared_ptr<Book> book;
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);
	bool parseToPayload();

	//Response
	static const unsigned int action_response = Protocol::ACTION_MODIFY_BOOK_RESPONSE;
	struct ResponseStruct
	{
		unsigned char success;
		std::string response;
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


#pragma once
#include "actiondatagramInterface.h"
class ActionBorrowBook :
	public ActionDatagramInterface
{
public:
	ActionBorrowBook();
	~ActionBorrowBook();

	//ACTION
	static const unsigned int action = Protocol::ACTION_BORROW_BOOK;
	struct PayloadStruct
	{
		unsigned int id;
		unsigned int userId;
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);
	bool parseToPayload();

	//Response
	static const unsigned int action_response = Protocol::ACTION_BORROW_BOOK_RESPONSE;
	struct ResponseStruct
	{
		unsigned char success;
		std::string response;
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


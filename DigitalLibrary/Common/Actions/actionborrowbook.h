#pragma once
#include "actiondatagramInterface.h"
#include "..\User.h"
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
		std::string username;
		std::shared_ptr<User> u; 
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


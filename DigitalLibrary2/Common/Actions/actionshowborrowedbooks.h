#pragma once
#include "actiondatagramInterface.h"
#include "../book.h"
class ActionShowBorrowedBooks :
	public ActionDatagramInterface
{
public:
	ActionShowBorrowedBooks();
	~ActionShowBorrowedBooks();

	//ACTION
	static const unsigned int action = Protocol::ACTION_SHOW_BORROWED_BOOKS;
	struct PayloadStruct
	{
		//no further information needs to be passed
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);//not needed but should be called for consistency of code
	bool parseToPayload();//still needed to call

	//Response
	static const unsigned int action_response = Protocol::ACTION_SHOW_BORROWED_BOOKS_RESPONSE;
	struct ResponseStruct
	{
		//values are set by response_parseToStruct / response_parseToPayload
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


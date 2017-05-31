#pragma once
#include "actiondatagramInterface.h"
#include "../book.h"
#include <map>
class ActionShowBooks :
	public ActionDatagramInterface
{
public:
	ActionShowBooks();
	~ActionShowBooks();

	//ACTION
	static const unsigned int action = Protocol::ACTION_SHOW_BOOKS;
	struct PayloadStruct
	{
		//no further information needs to be passed
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);//not needed but should be called for consistency of code
	bool parseToPayload();//still needed to call

	//Response
	static const unsigned int action_response = Protocol::ACTION_SHOW_BOOKS_RESPONSE;
	struct ResponseStruct
	{
		unsigned int size;//resetted in parseToPayload no need to set earlier
		//value is static member use Book::books instead
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


#pragma once
#include "actiondatagramInterface.h"
class ActionDeleteBook :
	public ActionDatagramInterface
{
public:
	ActionDeleteBook();
	~ActionDeleteBook();


	//ACTION
	static const unsigned int action = Protocol::ACTION_DELETE_BOOK;
	struct PayloadStruct
	{
		unsigned int id;
	};
	PayloadStruct payload_struct;
	std::shared_ptr<char> payload;
	unsigned int payload_size = 0;
	unsigned int payload_following = 0;
	unsigned char payload_action;
	bool parseToStruct(std::shared_ptr<char> newPayload);

	bool parseToPayload();

	//Response
	static const unsigned int action_response = Protocol::ACTION_DELETE_BOOK_RESPONSE;
	struct ResponseStruct
	{
		unsigned char success;
		std::string response;
	};
	ResponseStruct response_struct;
	std::shared_ptr<char> response_payload;
	unsigned int response_size = 0;
	unsigned int response_following = 0;
	unsigned char response_action;
	bool response_parseToStruct(std::shared_ptr<char> newPayload);

	bool response_parseToPayload();
};


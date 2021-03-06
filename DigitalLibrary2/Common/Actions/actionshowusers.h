#pragma once
#include "actiondatagramInterface.h"
class ActionShowUsers :
	public ActionDatagramInterface
{
public:
	ActionShowUsers();
	~ActionShowUsers();

	//ACTION
	static const unsigned int action = Protocol::ACTION_SHOW_USERS;
	struct PayloadStruct
	{


	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);
	bool parseToPayload();

	//Response
	static const unsigned int action_response = Protocol::ACTION_SHOW_USERS_RESPONSE;
	struct ResponseStruct
	{
		//User users[];
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


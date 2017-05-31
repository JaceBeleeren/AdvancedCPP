#pragma once
#include "actiondatagramInterface.h"
#include "..\user.h"
class ActionCreateUser :
	public ActionDatagramInterface
{
public:
	ActionCreateUser();
	~ActionCreateUser();

	//ACTION
	static const unsigned int action = Protocol::ACTION_CREATE_USER;
	struct PayloadStruct
	{
		std::shared_ptr<user> user;
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);
	bool parseToPayload();

	//Response
	static const unsigned int action_response = Protocol::ACTION_CREATE_USER_RESPONSE;
	struct ResponseStruct
	{
		unsigned int id;
		std::string response;
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


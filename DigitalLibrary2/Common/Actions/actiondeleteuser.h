#pragma once
#include "actiondatagramInterface.h"
#include "..\user.h"

class ActionDeleteUser :
	public ActionDatagramInterface
{
public:
	ActionDeleteUser();
	~ActionDeleteUser();

	//ACTION
	static const unsigned int action = Protocol::ACTION_DELETE_USER;
	struct PayloadStruct
	{
		unsigned int id;
		std::shared_ptr<user> user;
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);
	bool parseToPayload();

	//Response
	static const unsigned int action_response = Protocol::ACTION_DELETE_USER_RESPONSE;
	struct ResponseStruct
	{
		unsigned char success;
		std::string response;
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


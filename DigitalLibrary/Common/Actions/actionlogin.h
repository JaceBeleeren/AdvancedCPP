#pragma once
#include "actiondatagramInterface.h"
class ActionLogin :
	public ActionDatagramInterface
{
public:
	ActionLogin();
	~ActionLogin();

	//ACTION
	static const unsigned int action = Protocol::ACTION_LOGIN;
	struct PayloadStruct
	{
		std::string user;
		std::string password;
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);
	bool parseToPayload();

	//Response
	static const unsigned int action_response = Protocol::ACTION_LOGIN_RESPONSE;
	struct ResponseStruct
	{
		unsigned int id;
		unsigned int permissions;
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


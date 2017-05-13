#pragma once
#include "actiondatagramInterface.h"
class ActionLogout :
	public ActionDatagramInterface
{
public:
	ActionLogout();
	~ActionLogout();

	//ACTION
	static const unsigned int action = Protocol::ACTION_LOGOUT;
	struct PayloadStruct
	{
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);
	bool parseToPayload();

	//Response
	static const unsigned int action_response = Protocol::ACTION_LOGOUT_RESPONSE;
	struct ResponseStruct
	{
		unsigned char success;
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


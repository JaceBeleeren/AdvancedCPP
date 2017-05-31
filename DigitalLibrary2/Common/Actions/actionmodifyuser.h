#pragma once
#include "actiondatagramInterface.h"
#include "..\user.h"
class ActionModifyUser :
	public ActionDatagramInterface
{
public:
	ActionModifyUser();
	~ActionModifyUser();

	//ACTION
	static const unsigned int action = Protocol::ACTION_MODIFY_USER;
	struct PayloadStruct
	{
		std::shared_ptr<user> user;
		/*
		unsigned int id;
		std::string user;
		std::string password;
		unsigned int permissions;
		*/
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);
	bool parseToPayload();

	//Response
	static const unsigned int action_response = Protocol::ACTION_MODIFY_USER_RESPONSE;
	struct ResponseStruct
	{
		unsigned char success;
		std::string response;
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


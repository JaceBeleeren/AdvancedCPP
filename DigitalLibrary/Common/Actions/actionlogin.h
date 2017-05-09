#pragma once
#include "actiondatagramInterface.h"
class ActionLogin :
	public ActionDatagramInterface
{
public:
	ActionLogin();
	~ActionLogin();

	//ACTION
	struct PayloadStruct
	{
		std::string user;
		std::string password;
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);
	bool parseToPayload();

	//Response
	struct ResponseStruct
	{
		unsigned int id;
		unsigned int permissions;
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


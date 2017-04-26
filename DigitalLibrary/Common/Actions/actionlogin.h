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
		unsigned int test;
		std::string string;
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> datagram);
	bool parseToPayload();

	//Response
	struct ResponseStruct
	{
		unsigned int id;
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> datagram);
	bool response_parseToPayload();
};


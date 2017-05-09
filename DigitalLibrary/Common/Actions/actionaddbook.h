#pragma once
#include "actiondatagramInterface.h"
class ActionAddBook :
	public ActionDatagramInterface
{
public:
	ActionAddBook();
	~ActionAddBook();

	//ACTION
	struct PayloadStruct
	{
		std::string title;
		std::string author;
		std::string summary;
		std::string publisher;
		unsigned int year;
		std::string isbn;
		unsigned int amount;
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);
	bool parseToPayload();

	//Response
	struct ResponseStruct
	{
		unsigned int id; //new or existing id
		std::string response; //can used to tell that book exists already for example
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};


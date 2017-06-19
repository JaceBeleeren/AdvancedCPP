#pragma once
#include "actiondatagramInterface.h"
#include "../book.h"
#include <map>
#include <list>

class ActionSearchBook :
	public ActionDatagramInterface
{
public:
	ActionSearchBook();
	~ActionSearchBook();

	//ACTION
	static const unsigned int action = Protocol::ACTION_SEARCH_BOOK;
	struct PayloadStruct
	{
		
		std::string title; 
		std::string author; 
		std::string publisher; 
		int year; 
	};
	PayloadStruct payload_struct;
	bool parseToStruct(std::shared_ptr<char> payload);//not needed but should be called for consistency of code
	bool parseToPayload();//still needed to call

						  //Response
	static const unsigned int action_response = Protocol::ACTION_SEARCH_BOOK_RESPONSE;
	struct ResponseStruct
	{
		unsigned int size;//resetted in parseToPayload no need to set earlier
						  //value is static member use Book::books instead
	std::shared_ptr<Book> b; 
	//static std::list<std::shared_ptr<Book>> listBooks; 
	std::list<Book> listBooks;
	};
	ResponseStruct response_struct;
	bool response_parseToStruct(std::shared_ptr<char> payload);
	bool response_parseToPayload();
};
 

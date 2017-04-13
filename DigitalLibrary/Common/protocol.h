#pragma once

#include <iostream>
#include <boost/asio.hpp>

class Protocol
{
public:
	static bool decode_header(unsigned int *payload_size, unsigned char *action, unsigned char *following, char *header);
	static void encode_header(unsigned int payload_size, unsigned char action, unsigned char following, char *data);

	static unsigned int charToInt(char* data);
	static void intToChar(unsigned int data, char* result);

	static const unsigned short PORT = 12121;
	static const std::string SERVICE;

	static const int HEADER_PAYLOAD_SIZE = 4;
	static const int HEADER_ACTION_SIZE = 2;
	static const int HEADER_SIZE = HEADER_PAYLOAD_SIZE + HEADER_ACTION_SIZE;
	static const int MAX_PAYLOAD_SIZE = 9999;
	static const unsigned int MAX_ERRORCOUNT = 100;
	static const unsigned int MAX_RECONNECTCOUNT = 10;

	static const int ACTION_LOGIN = 1;
	static const int ACTION_LOGIN_RESPONSE = 2;
	static const int ACTION_LIST_BOOKS = 3;
	static const int ACTION_LIST_BOOKS_RESPONSE = 4;

	struct Login
	{
		std::string user;
	};
	static Login login_parseToStruct(char* payload);
	static char* login_parseToPayload(Login input);

	struct Login_Response
	{
		int id;
	};
	static Login_Response login_response_parseToStruct(char* payload);
	static char* login_response_parseToPayload(Login_Response input);

	//not needed since no payload is submitted
	/*
	struct List_Books
	{
	};
	*/

	struct Book
	{
		int id;
		std::string title;
		std::string author;
	};

	struct List_Books_RESPONSE
	{
		Book *books;
	};

private:
	Protocol();
	~Protocol();
};


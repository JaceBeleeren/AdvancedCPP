#pragma once

#include <iostream>
#include <boost/asio.hpp>

class Protocol
{
public:
	template< typename T >
	struct array_deleter
	{
		void operator ()(T const * p)
		{
			delete[] p;
		}
	};

	static bool decode_header(unsigned int *payload_size, unsigned char *action, unsigned char *following, char *header);
	static void encode_header(unsigned int payload_size, unsigned char action, unsigned char following, char *data);

	static unsigned int charToUInt(char* data);
	static void uintToChar(unsigned int data, char* result);

	static const unsigned short PORT = 12121;
	static const std::string SERVICE;

	static const unsigned int HEADER_PAYLOAD_SIZE = 4;
	static const unsigned int HEADER_ACTION_SIZE = 2;
	static const unsigned int HEADER_SIZE = HEADER_PAYLOAD_SIZE + HEADER_ACTION_SIZE;
	static const unsigned int MAX_PAYLOAD_SIZE = 9999;
	static const unsigned int MAX_ERRORCOUNT = 100;
	static const unsigned int MAX_RECONNECTCOUNT = 10;

	static const unsigned int ACTION_LOGIN = 1;
	static const unsigned int ACTION_LOGIN_RESPONSE = 2;
	static const unsigned int ACTION_LIST_BOOKS = 3;
	static const unsigned int ACTION_LIST_BOOKS_RESPONSE = 4;
	static const unsigned int ACTION_LOGOUT = 5;
	static const unsigned int ACTION_LOGOUT_RESPONSE = 6;
	static const unsigned int ACTION_ADD_BOOK = 8;
	static const unsigned int ACTION_MODIFIY_BOOK = 9;

private:
	Protocol();
	~Protocol();
};


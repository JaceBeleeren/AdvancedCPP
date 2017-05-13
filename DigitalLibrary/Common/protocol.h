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

	static bool decode_header(unsigned int *payload_size, unsigned char *action, unsigned char *following, std::shared_ptr<char> header);
	static void encode_header(unsigned int payload_size, unsigned char action, unsigned char following, std::shared_ptr<char> data);

	static unsigned int charToUInt(char* data);
	static void uintToChar(unsigned int data, char* result);

	static unsigned int charToInt(char* data);
	static void intToChar(int data, char* result);

	static const unsigned short SERVERPORT = 12121;
	static const std::string SERVICE;

	static const unsigned int HEADER_PAYLOAD_SIZE = 4;
	static const unsigned int HEADER_ACTION_SIZE = 2;
	static const unsigned int HEADER_SIZE = HEADER_PAYLOAD_SIZE + HEADER_ACTION_SIZE;
	static const unsigned int MAX_PAYLOAD_SIZE = 9999;
	static const unsigned int MAX_ERRORCOUNT = 100;
	static const unsigned int MAX_RECONNECTCOUNT = 10;

	static const unsigned char ACTION_LOGIN = 10;
	static const unsigned char ACTION_LOGIN_RESPONSE = 11;
	static const unsigned char ACTION_MODIFY_BOOK = 20;
	static const unsigned char ACTION_MODIFY_BOOK_RESPONSE = 21;
	static const unsigned char ACTION_BORROW_BOOK = 30;
	static const unsigned char ACTION_BORROW_BOOK_RESPONSE = 31;
	static const unsigned char ACTION_CREATE_USER = 40;
	static const unsigned char ACTION_CREATE_USER_RESPONSE = 41;
	static const unsigned char ACTION_DELETE_USER = 50;
	static const unsigned char ACTION_DELETE_USER_RESPONSE = 51;
	static const unsigned char ACTION_LOGOUT = 60;
	static const unsigned char ACTION_LOGOUT_RESPONSE = 61;
	static const unsigned char ACTION_MODIFY_USER = 70;
	static const unsigned char ACTION_MODIFY_USER_RESPONSE = 71;
	static const unsigned char ACTION_SHOW_BOOKS = 80;
	static const unsigned char ACTION_SHOW_BOOKS_RESPONSE = 81;
	static const unsigned char ACTION_SHOW_BORROWED_BOOKS = 90;
	static const unsigned char ACTION_SHOW_BORROWED_BOOKS_RESPONSE = 91;
	static const unsigned char ACTION_SHOW_USERS = 100;
	static const unsigned char ACTION_SHOW_USERS_RESPONSE = 101;
	static const unsigned char ACTION_ADD_BOOK = 110;
	static const unsigned char ACTION_ADD_BOOK_RESPONSE = 111;
	static const unsigned char ACTION_DELETE_BOOK = 120;
	static const unsigned char ACTION_DELETE_BOOK_RESPONSE = 121;

private:
	Protocol();
	~Protocol();
};


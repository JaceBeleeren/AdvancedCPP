#include "protocol.h"

const std::string Protocol::SERVICE = std::to_string(Protocol::PORT);
Protocol::Protocol()
{
}


Protocol::~Protocol()
{
}

bool Protocol::decode_header(unsigned int *payload_size, unsigned char *action, unsigned char *following, char *header)
{
	*payload_size = charToInt(header);
	//convert header (char[4]) to int
	/*
	int temp;
	for (int i = 0; i < 4; i++)
	{
		temp = reinterpret_cast<unsigned char&>(header[i]);//header is saved as signed char, but unsigned char (bit value) is needed
		*payload_size += temp << ((3 - i) * 8);
	}*/
	if (*payload_size > MAX_PAYLOAD_SIZE)
	{
		std::cout << "Payloadsize:" << *payload_size << " > " << MAX_PAYLOAD_SIZE << std::endl;
		*payload_size = 0;
		return false;
	}
	*action = reinterpret_cast<unsigned char&>(header[HEADER_PAYLOAD_SIZE]);
	*following = reinterpret_cast<unsigned char&>(header[HEADER_PAYLOAD_SIZE+1]);
	return true;
}

void Protocol::encode_header(unsigned int payload_size, unsigned char action, unsigned char following, char *data)
{
	intToChar(payload_size, data);
	/*unsigned char temp;
	for (int i = 0; i < 4; i++)
	{
		temp = payload_size >> (i * 8);
		data[3 - i] = reinterpret_cast<signed char&>(temp);//header is saved as signed char, but unsigned char (bit value) is needed
	}*/
	data[HEADER_PAYLOAD_SIZE] = reinterpret_cast<signed char&>(action);
	data[HEADER_PAYLOAD_SIZE+1] = reinterpret_cast<signed char&>(following);
}

//converts first 4 bytes of char* data to unsigned int and returns the value
unsigned int Protocol::charToInt(char* data)
{
	int result = 0;
	int temp;
	for (int i = 0; i < 4; i++)
	{
		temp = reinterpret_cast<unsigned char&>(data[i]);//data is saved as signed char, but unsigned char (bit value) is needed
		result += temp << ((3 - i) * 8);
	}
	return result;
}

//converts unsigned int data to 4 bytes of char* and saves them in the first 4 bytes of result
void Protocol::intToChar(unsigned int data, char* result)
{
	unsigned char temp;
	for (int i = 0; i < 4; i++)
	{
		temp = data >> (i * 8);
		result[3 - i] = reinterpret_cast<signed char&>(temp);//data is saved as signed char, but unsigned char (bit value) is needed
	}
}

Protocol::Login Protocol::login_parseToStruct(char* payload)
{
	Login result;
	result.user = std::string(payload);
	return result;
}

char* Protocol::login_parseToPayload(Login input)
{
	char data[Protocol::MAX_PAYLOAD_SIZE + 1];
	std::copy(input.user.begin(), input.user.end(), data);
	data[input.user.size()] = '\0';
	return data;
}

Protocol::Login_Response Protocol::login_response_parseToStruct(char* payload)
{
	Login_Response result;
	result.id = charToInt(payload);
	return result;
}

char* Protocol::login_response_parseToPayload(Login_Response input)
{
	char data[Protocol::MAX_PAYLOAD_SIZE + 1];
	intToChar(input.id, data);//4bytes
	data[5] = '\0';//last byte = 5
	return data;
}

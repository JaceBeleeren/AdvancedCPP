#include "protocol.h"

const std::string Protocol::SERVICE = std::to_string(Protocol::SERVERPORT);
Protocol::Protocol()
{
}


Protocol::~Protocol()
{
}

bool Protocol::decode_header(unsigned int *payload_size, unsigned char *action, unsigned char *following, std::shared_ptr<char> header)
{
	*payload_size = charToUInt(header.get());
	if (*payload_size > MAX_PAYLOAD_SIZE)
	{
		std::cout << "Payloadsize:" << *payload_size << " > " << MAX_PAYLOAD_SIZE << std::endl;
		*payload_size = 0;
		return false;
	}
	*action = reinterpret_cast<unsigned char&>(header.get()[HEADER_PAYLOAD_SIZE]);
	*following = reinterpret_cast<unsigned char&>(header.get()[HEADER_PAYLOAD_SIZE+1]);
	return true;
}

void Protocol::encode_header(unsigned int payload_size, unsigned char action, unsigned char following, std::shared_ptr<char> data)
{
	uintToChar(payload_size, data.get());
	data.get()[HEADER_PAYLOAD_SIZE] = reinterpret_cast<signed char&>(action);
	data.get()[HEADER_PAYLOAD_SIZE+1] = reinterpret_cast<signed char&>(following);
}

//converts first 4 bytes of char* data to unsigned int and returns the value
unsigned int Protocol::charToUInt(char* data)
{
	int uint_size = sizeof(unsigned int);
	int result = 0;
	int temp;
	for (int i = 0; i < uint_size; i++)
	{
		temp = reinterpret_cast<unsigned char&>(data[i]);//data is saved as signed char, but unsigned char (bit value) is needed
		result += temp << ((uint_size - 1 - i) * 8);
	}
	return result;
}

//converts unsigned int data to 4 bytes of char* and saves them in the first 4 bytes of result
void Protocol::uintToChar(unsigned int data, char* result)
{
	int uint_size = sizeof(unsigned int);
	unsigned char temp;
	for (int i = 0; i < uint_size; i++)
	{
		temp = data >> (i * 8);
		result[uint_size - 1 - i] = reinterpret_cast<signed char&>(temp);//data is saved as signed char, but unsigned char (bit value) is needed
	}
}

//converts first 4 bytes of char* data to int and returns the value
unsigned int Protocol::charToInt(char* data)
{
	int int_size = sizeof(int);
	int result = 0;
	int temp;
	for (int i = 0; i < int_size; i++)
	{
		temp = reinterpret_cast<unsigned char&>(data[i]);//data is saved as signed char, but unsigned char (bit value) is needed
		result += temp << ((int_size - 1 - i) * 8);
	}
	return result;
}

//converts int data to 4 bytes of char* and saves them in the first 4 bytes of result
void Protocol::intToChar(int data, char* result)
{
	int int_size = sizeof(int);
	unsigned char temp;
	for (int i = 0; i < int_size; i++)
	{
		temp = data >> (i * 8);
		result[int_size - 1 - i] = reinterpret_cast<signed char&>(temp);//data is saved as signed char, but unsigned char (bit value) is needed
	}
}
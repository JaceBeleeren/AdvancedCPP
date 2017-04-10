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
	*payload_size = 0;
	//convert header (char[4]) to int
	int temp;
	for (int i = 0; i < 4; i++)
	{
		temp = reinterpret_cast<unsigned char&>(header[i]);//header is saved as signed char, but unsigned char (bit value) is needed
		*payload_size += temp << ((3 - i) * 8);
	}
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
	unsigned char temp;
	for (int i = 0; i < 4; i++)
	{
		temp = payload_size >> (i * 8);
		data[3 - i] = reinterpret_cast<signed char&>(temp);//header is saved as signed char, but unsigned char (bit value) is needed
	}
	data[HEADER_PAYLOAD_SIZE] = reinterpret_cast<signed char&>(action);
	data[HEADER_PAYLOAD_SIZE+1] = reinterpret_cast<signed char&>(following);
}

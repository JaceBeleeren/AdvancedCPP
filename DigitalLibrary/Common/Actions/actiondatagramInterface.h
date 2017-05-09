#pragma once

#include "..\protocol.h"

class ActionDatagramInterface
{
public:
	ActionDatagramInterface() {};
	~ActionDatagramInterface() {};

	//ACTION
	virtual struct PayloadStruct
	{
		//fill with needed data
		//can also be objects (beware of correct handling while parsing!)
	};
	std::shared_ptr<char> payload;
	unsigned int payload_size = 0;
	virtual bool parseToStruct(std::shared_ptr<char> newPayload)
	{
		payload = newPayload;
		payload_size = 0;
		//copy string from payload
		/*
		payload_struct.string = std::string(payload.get() + payload_size);
			payload_size = payload_struct.user.size() + 1; //+1 for null terminator
		*/
		//copy unsigned int from payload
		/*
		payload_struct.uint = Protocol::charToUInt(payload.get() + payload_size);
			payload_size += sizeof(unsigned int);
		*/
		return true;
	};
	virtual bool parseToPayload()
	{
		payload_size = 0;
		if (!payload)
			payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

		//copy string to payload
		/*
		std::copy(payload_struct.string.begin(), payload_struct.string.end(), payload.get() + payload_size);
			payload_size += payload_struct.string.size();
				payload.get()[payload_size] = '\0';
				payload_size += 1;
		*/
		//copy unsigned int to payload
		/*
			Protocol::uintToChar(payload_struct.uint, payload.get() + payload_size);//4bytes
			payload_size += sizeof(unsigned int);
		*/
		return true;
	};

	//Response
	virtual struct ResponseStruct
	{
	};
	std::shared_ptr<char> response_payload;
	unsigned int response_size = 0;
	virtual bool response_parseToStruct(std::shared_ptr<char> newPayload)
	{
		response_payload = newPayload;
		response_size = 0;
		//see parseToStruct()
		return true;
	};
	virtual bool response_parseToPayload()
	{
		response_size = 0;
		if (!response_payload)
			response_payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());
		//see parseToPayload()
		return true;
	};
	virtual void fillResponsePayloadStruct() {};
};

/*
Examples:
Sending Data:

	ActionLogin payload;//extends ActionDatagramInterface
	//Fill payload with needed data
		payload.payload_struct.user = "Erwin aus dem Wald";
		payload.payload_struct.test = 12345;
		payload.payload_struct.string = "abcdefg";
		payload.parseToPayload();
	//create array for header AND payload
		char data[Protocol::HEADER_SIZE + Protocol::MAX_PAYLOAD_SIZE + 1];
	//generate header
		Protocol::encode_header(payload.payload_size, Protocol::ACTION_LOGIN, 0, data);
	//copy payload into datagram array
		memcpy(data + Protocol::HEADER_SIZE, payload.payload.get(), payload.payload_size);

	boost::asio::async_write
	(
		socket,
		boost::asio::buffer(data, Protocol::HEADER_SIZE + payload.payload_size),
		boost::bind(&TCP_Client::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
	);

Receiving Data:
	std::shared_ptr<char> payload;
	unsigned int payload_size;
	unsigned char action;
	unsigned char following;
	char header[Protocol::HEADER_SIZE];

	void TCP_Connection::handle_read_header(const boost::system::error_code& error, std::size_t n)
	{
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());
		if (!error && Protocol::decode_header(&payload_size, &action, &following, header))
		{
			boost::asio::async_read
			(
				socket_,
				boost::asio::buffer(payload.get(), payload_size),
				boost::bind(&TCP_Connection::handle_read_payload, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
			);
		}
	}

	void TCP_Connection::handle_read_payload(const boost::system::error_code& error, std::size_t n)
	{
		if (!error)
		{
			payload.get()[payload_size] = 0; //force end payload with null terminator
			if (action == Protocol::ACTION_LOGIN)
			{
				ActionLogin login;
				login.parseToStruct(datagram);
				std::cout << "User: " << login.payload_struct.user << std::endl;
				std::cout << "Test: " << login.payload_struct.test << std::endl;
				std::cout << "String: " << login.payload_struct.string << std::endl;
			}
		}
	}
*/

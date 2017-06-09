#include "actionshowborrowedbooks.h"



ActionShowBorrowedBooks::ActionShowBorrowedBooks()
{
}


ActionShowBorrowedBooks::~ActionShowBorrowedBooks()
{
}


bool ActionShowBorrowedBooks::parseToStruct(std::shared_ptr<char> newPayload)
{
	payload = newPayload;
	payload_size = 0;

	/*
	nothing to be done here
	*/

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionShowBorrowedBooks::parseToPayload()
{
	payload_size = 0;
	if (!payload)
		payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	/*
	nothing to be done here
	*/

	if (payload_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionShowBorrowedBooks::response_parseToStruct(std::shared_ptr<char> newPayload)
{
	response_payload = newPayload;
	response_size = 0;

	Book::books.empty(); //create new map of needed books get by server
	//User::users.empty(); //create new map of needed users get by server

	unsigned int sizeBooks; //number of books received by server
	unsigned int sizeUsers; //number of users received by server

	//unsigned int sizeBooks
	if (!check_response_size(response_size))
		return false;
	sizeBooks = Protocol::charToUInt(response_payload.get() + response_size);
	response_size += sizeof(unsigned int);

	for (unsigned int i = 0; i < sizeBooks; i++)//iterate over every book received
	{
		//std::string title
		if (!check_response_size(response_size))
			return false;
		std::string title = std::string(response_payload.get() + response_size);
		response_size = title.size() + sizeof(char);

		//std::string author
		if (!check_response_size(response_size))
			return false;
		std::string author = std::string(response_payload.get() + response_size);
		response_size += author.size() + sizeof(char);

		//std::string summary
		if (!check_response_size(response_size))
			return false;
		std::string summary = std::string(response_payload.get() + response_size);
		response_size += summary.size() + sizeof(char);

		//std::string publisher
		if (!check_response_size(response_size))
			return false;
		std::string publisher = std::string(response_payload.get() + response_size);
		response_size += publisher.size() + sizeof(char);

		//unsigned int year
		if (!check_response_size(response_size))
			return false;
		unsigned int year = Protocol::charToUInt(response_payload.get() + response_size);
		response_size += sizeof(unsigned int);

		//std::string isbn
		if (!check_response_size(response_size))
			return false;
		std::string isbn = std::string(response_payload.get() + response_size);
		response_size += isbn.size() + sizeof(char);

		//unsigned int amount
		if (!check_response_size(response_size))
			return false;
		unsigned int amount = Protocol::charToUInt(response_payload.get() + response_size);
		response_size += sizeof(unsigned int);

		//unsigned int sizeBorrowedByUser //size of received map borrowedByUser
		if (!check_response_size(response_size))
			return false;
		unsigned int borrowedByUserSize = Protocol::charToUInt(response_payload.get() + response_size);
		response_size += sizeof(unsigned int);

		try
		{
			//insert new book received, throws exception if not successful
			std::shared_ptr<Book> book = std::shared_ptr<Book>(new Book());
			book.get()->constructorBook(title, author, summary, publisher, year, isbn, amount);

			for (unsigned int j = 0; j < borrowedByUserSize; j++)//iterate over every user in received map borrowedByUser
			{
				//unsigned int userId
				if (!check_response_size(response_size))
					return false;
				unsigned int userId = Protocol::charToUInt(response_payload.get() + response_size);
				response_size += sizeof(unsigned int);

				//unsigned int amount Borrowed
				if (!check_response_size(response_size))
					return false;
				unsigned int amountBorrowed = Protocol::charToUInt(response_payload.get() + response_size);
				response_size += sizeof(unsigned int);

				book.get()->borrowedByUser.insert(std::make_pair(userId, amountBorrowed));//insert new userId/amountBorroed-pair
			}
		}
		catch (std::exception& e)
		{
			//creating new book failed
			//message printed in book constructor
			//return false;
		}
	}

	//unsigned int sizeUsers
	if (!check_response_size(response_size))
		return false;
	sizeUsers = Protocol::charToUInt(response_payload.get() + response_size);
	response_size += sizeof(unsigned int);

	/*
	for (int i; i <= response_struct.sizeUsers; i++)//iterate over every user received
	{
		/ *get needed parameter to create User-object* /
		//std::string
		if (!check_response_size(response_size))
			return false;
		std::string string = std::string(response_payload.get() + response_size);
		response_size = response_struct.string.size() + sizeof(char);

		//unsigned int
		if (!check_response_size(response_size))
			return false;
		unsigned int uint = Protocol::charToUInt(response_payload.get() + response_size);
		response_size += sizeof(unsigned int);

		//int
		if (!check_response_size(response_size))
			return false;
		int int = Protocol::charToUInt(response_payload.get() + response_size);
		response_size += sizeof(unsigned int);

		//char/unsigned char
		if (!check_response_size(response_size))
			return false;
		char char = *(response_payload.get() + response_size);
		response_size += sizeof(char);

		try
		{ 
			//insert new user received, throws exception if not successful
			std::shared_ptr<User> user = std::shared_ptr<User>(new User(/ *needed parameters* /));
		}
		catch (std::exception& e)
		{
			//message printed in book constructor
			//return false;
		}
	}
	*/

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}

bool ActionShowBorrowedBooks::response_parseToPayload()
{
	unsigned int add;
	response_size = 0;
	if (!response_payload)
		response_payload = std::shared_ptr<char>(new char[Protocol::MAX_PAYLOAD_SIZE + 1], Protocol::array_deleter<char>());

	unsigned int sizeBooks = 0; //number of books send to client
	unsigned int sizeUsers = 0; //number of users send to client

	//unsigned int sizeBooks
	add = sizeof(unsigned int);
	if (!check_response_size(response_size + add))
		return false;
	unsigned int sizeBooksAdress = response_size;
	//Protocol::uintToChar(sizeBooks, response_payload.get() + response_size);//cant be written yet since size is not known yet
	response_size += add;

	//std::map<unsigned int, std::shared_ptr<User>> tempUsers;

	//add needed books to response_payload
	std::map<unsigned int, std::shared_ptr<Book>>::iterator itBooks;
	for (itBooks = Book::books.begin(); itBooks != Book::books.end(); itBooks++)
	{
		if (itBooks->second.get()->getBorrowed() <= 0)
			break; //no need to add book with no borrowed books

		sizeBooks++;//increment number of books

		//unsigned int id
		add = sizeof(unsigned int);
		if (!check_response_size(response_size + add))
			return false;
		Protocol::uintToChar(itBooks->second.get()->getId(), response_payload.get() + response_size);//4bytes
		response_size += add;

		//std::string title
		add = itBooks->second.get()->title.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(itBooks->second.get()->title.begin(), itBooks->second.get()->title.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//std::string author
		add = itBooks->second.get()->author.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(itBooks->second.get()->author.begin(), itBooks->second.get()->author.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//std::string summary
		add = itBooks->second.get()->summary.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(itBooks->second.get()->summary.begin(), itBooks->second.get()->summary.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//std::string publisher
		add = itBooks->second.get()->publisher.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(itBooks->second.get()->publisher.begin(), itBooks->second.get()->publisher.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//unsigned int year
		add = sizeof(unsigned int);
		if (!check_response_size(response_size + add))
			return false;
		Protocol::uintToChar(itBooks->second.get()->year, response_payload.get() + response_size);//4bytes
		response_size += add;

		//std::string isbn
		add = itBooks->second.get()->getIsbn().size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(itBooks->second.get()->getIsbn().begin(), itBooks->second.get()->getIsbn().end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//unsigned int amount
		add = sizeof(unsigned int);
		if (!check_response_size(response_size + add))
			return false;
		Protocol::uintToChar(itBooks->second.get()->getAmount(), response_payload.get() + response_size);//4bytes
		response_size += add;

		//unsigned int sizeBorrowedByUser //size of map borrowedByUser
		add = sizeof(unsigned int);
		if (!check_response_size(response_size + add))
			return false;
		unsigned int borrowedByUserAdress = response_size;
		unsigned int borrowedByUserSize = 0;
		//Protocol::uintToChar(borrowedByUserSize, response_payload.get() + response_size);//set after we know how big is the map
		response_size += add;

		std::map<unsigned int, unsigned int>::iterator itBorrowedByUser;//key: userID value: amount borrowed
		for (itBorrowedByUser = itBooks->second.get()->borrowedByUser.begin(); itBorrowedByUser != itBooks->second.get()->borrowedByUser.end(); itBorrowedByUser++)
		{
			if (itBorrowedByUser->second <= 0)
				break; //not needed to add

			//insert user with amountBorrowed to map borrowedByUser

			//unsigned int key/userId
			add = sizeof(unsigned int);
			if (!check_response_size(response_size + add))
				return false;
			Protocol::uintToChar(itBorrowedByUser->first, response_payload.get() + response_size);//4bytes
			response_size += add;

			//unsigned int amount
			add = sizeof(unsigned int);
			if (!check_response_size(response_size + add))
				return false;
			Protocol::uintToChar(itBorrowedByUser->second, response_payload.get() + response_size);//4bytes
			response_size += add;

			borrowedByUserSize++;

			//insert user to tempUsersMap (users to be submitted to client)
			/*
			if(tempUsers.find(itBorrowedByUser->first) != tempUsers.end())
					tempUsers.insert(std::make_pair(itBorrowedByUser->first, User::getUser(itBorrowedByUser->first)));
			*/
		}

		//unsigned int sizeBorrowedByUser is known and can finally be written
		Protocol::uintToChar(borrowedByUserSize, response_payload.get() + borrowedByUserAdress);
	}

	//unsigned int sizeBooks can finally be written
	Protocol::uintToChar(sizeBooks, response_payload.get() + sizeBooksAdress);

	//unsigned int sizeUsers
	add = sizeof(unsigned int);
	if (!check_response_size(response_size + add))
		return false;
	unsigned int sizeUsersAdress = response_size;
	//Protocol::uintToChar(sizeUsers, response_payload.get() + response_size);//cant be written yet since size is not known yet
	response_size += add;

	//add needed users to response_payload
	/*
	std::map<unsigned int, std::shared_ptr<User>>::iterator itUsers;
	for (itUsers = tempUsers.begin(); itUsers != tempUsers.end(); itUsers++)
	{

		//std::string
		add = response_struct.string.size();
		if (!check_response_size(response_size + add))
			return false;
		std::copy(itUsers->second.get()->string.begin(), itUsers->second.get()->string.end(), response_payload.get() + response_size);
		response_size += add;

		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = '\0';
		response_size += add;

		//unsigned int
		add = sizeof(unsigned int);
		if (!check_response_size(response_size + add))
			return false;
		Protocol::uintToChar(itUsers->second.get()->uint, itUsers->second.get()->get() + response_size);//4bytes
		response_size += add;

		//int
		add = sizeof(int);
		if (!check_response_size(response_size + add))
			return false;
		Protocol::uintToChar(itUsers->second.get()->int, response_payload.get() + response_size);//4bytes
		response_size += add;

		//char/unsigned char
		add = sizeof(char);
		if (!check_response_size(response_size + add))
			return false;
		response_payload.get()[response_size] = itUsers->second.get()->char;
		response_size += add;
	}
	*/
	
	//unsigned int sizeUsers can finally be written
	Protocol::uintToChar(sizeUsers, response_payload.get() + sizeUsersAdress);//cant be written yet since size is not known yet

	if (response_size > Protocol::MAX_PAYLOAD_SIZE)
		return false;
	return true;
}
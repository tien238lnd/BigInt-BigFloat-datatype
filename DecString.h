#pragma once
#include <string>

class DecString
{
	static const int NUMBYTES = 40;
	char bytes[NUMBYTES];		// "123456789012345678901234567890"

public:
	DecString(std::string src);
	DecString();

	bool isZero();
	
	//return the remainder
	bool dividedBy2();
	void multipliedBy2();
	void added1();

	std::string getString();
};



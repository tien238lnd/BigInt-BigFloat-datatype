#pragma once
#include <string>

//class DecString use to handle the conversion between decimal string and QInt
//"938657134268276439" to QInt
class DecString
{
	static const int NUMBYTES = 40;
	char bytes[NUMBYTES];

public:
	//default constructor
	//set to the zero value
	DecString();

	//Constructor from a decimal string 
	DecString(std::string src);

	//check if the number is a zero value
	bool isZero();

	//divide this DecString by 2, assign the result to itselt
	//return the remainder of the division
	bool dividedBy2();

	//multiply this DecString by 2, assign the result to itselt
	void multipliedBy2();

	//add 1 to this DecString
	void added1();

	//get the string to print of this DecString
	std::string getString();
};

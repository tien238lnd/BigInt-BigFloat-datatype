<<<<<<< HEAD
#include <iostream>
#include "QInt.h";
class Qfloat{
    char bytes[16];


public:
=======
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "DecString.h"

class Qfloat{
	//static const int NUMBYTES = 16;
	//static const int NUMBITS = 128;
    char bytes[128];	//		1/15/112

	//static const Qfloat pINF("0b0111111111111111");
	//static const Qfloat nINF("0b1111111111111111");
private:
>>>>>>> 8fcb0df2ff937b7c53c825a037fd4e3d93e27286
	//set the bit at position i the value b
	void setBit(char i, bool b);

	//get the value of the bit at position i
	bool getBit(char i) const;

	// use for output in decimal representation
	std::string toDecString() const;

	// use for output in binary representation



	void fromDecString(std::string src);

	void fromBinString(std::string src);	// easy
public:
	std::string toBinString() const;	// easy
	//default constructor, value is Zero
	Qfloat();

	//Copy constructor
	Qfloat(const Qfloat& src);

	//constructor with string in decimal or binary
	//in decimal, you can represent it like 100.001 or 6.022e23, -1.602e-19 (must be in normalized form)
	//in binary, it must have "0b" in the front
	Qfloat(std::string src);

	//constructor with const char*
	Qfloat(const char* src);

	//assign operator
	Qfloat& operator=(const Qfloat& src);

	Qfloat& operator=(std::string srcStr);

	Qfloat& operator=(const char* srcStr);

    //input a Qfloat qi from istream istr
    friend std::istream& operator>>(std::istream& istr, Qfloat& qf);

    //output a Qfloat qi to the ostream ostr
    friend std::ostream& operator<<(std::ostream& ostr, const Qfloat& qf);

// ----------------------------------------------------------
    void addexponent(int numadd);

    friend Qfloat operator+( Qfloat &x,  Qfloat &y) ;
    friend Qfloat operator-(const Qfloat &x, const Qfloat &y);
    friend Qfloat operator*(const Qfloat &x, const Qfloat &y);
    friend Qfloat operator/(const Qfloat &x, const Qfloat &y);




};

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "QInt.h"

class QFloat {
	static const int NUMBYTES = 16;
	static const int NUMBITS = 128;
	static const int BIAS = 16383;
	char bytes[NUMBYTES];	//		1/15/112

public:
	static const char Zero = 0;
	static const char Infinity = 1;
	static const char Nan = 2;
	static const char Denormalize = 3;
	static const char Normal = 4;

public:
	//set the bit at position i the value b (0 or 1)
	void setBit(char i, bool b);

	//return the value of the bit at position i
	bool getBit(char i) const;

	// use for output in decimal representation
	std::string toDecString() const;

	// use for output in binary representation
	std::string toBinString() const;

	//return an int hold the value of the exponent of this QFloat
	int get_exponent() const;

	//separate the integral part and fractional part
	void modf(QFloat& integral, QFloat& fractional) const;
	
	//From a QFloat that equal or nearly equal to a QFloat between 0 to 9 (nearly equal because of tolerance of calculating)
	//Return a char '0'->'9' corresponding to the input
	char toChar() const;

	//Convert from a string in base 10 to QFloat
	void fromDecString(std::string src);
	//convert from a string in base 2 to QFloat
	void fromBinString(std::string src);	// easy

public:
	//default constructor, value is Zero
	QFloat();

	//Copy constructor
	QFloat(const QFloat& src);

	//constructor with string in decimal or binary
	//in decimal, you can represent it like 100.001 or 6.022e23, -1.602e-19 (must be in normalized form)
	//in binary, it must have "0b" in the front
	QFloat(std::string src);

	//constructor with const char*
	QFloat(const char* src);

	//assign operator
	QFloat& operator=(const QFloat& src);

	//assign to a string that perform QFloat value
	QFloat& operator=(std::string srcStr);

	//assign to a const char array that perform QFloat value
	QFloat& operator=(const char* srcStr);

	//input a QFloat qi from istream istr
	friend std::istream& operator>>(std::istream& istr, QFloat& qf);

	//output a QFloat qi to the ostream ostr
	friend std::ostream& operator<<(std::ostream& ostr, const QFloat& qf);

	// ----------------------------------------------------------
	friend QFloat operator+(const QFloat& x, const QFloat& y);
	friend QFloat operator-(const QFloat& x, const QFloat& y);
	friend QFloat operator*(const QFloat& x, const QFloat& y);
	friend QFloat operator/(const QFloat& x, const QFloat& y);

};
//check if QFloat q is zero, inf, nan, denormalize or normalize
//return the type code that is above in the static const char variable
char CheckTypeofQFloat(const QFloat& q);
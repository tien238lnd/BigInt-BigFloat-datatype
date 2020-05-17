#pragma once
#include <iostream>
#include <fstream>
#include <string>


class QInt
{
	static const int NUMBYTES = 16;
	static const int NUMBITS = 128;
	char bytes[NUMBYTES];
public:

	//default constructor, value is Zero
	QInt();

	//Copy constructor
	QInt(const QInt& src);

	//constructor with string
	QInt(std::string src);

	//constructor with const char*
	QInt(const char* src);

	//assign operator
	QInt& operator=(const QInt& src);

	//assign operator, to a value descript by a string
	//operator accept value in both decimal
	//for negative decimal: have "-" sign in the front
	//for binary: srcStr have "0b" in the front
	//for hexadecimal: srcStr have "0x" in the front
	QInt& operator=(std::string srcStr);

	//assign operator, to a value descript by a const char
	//operator accept value in both decimal
	//for negative decimal: have "-" sign in the front
	//for binary: srcStr have "0b" in the front
	//for hexadecimal: srcStr have "0x" in the front
	QInt& operator=(const char* srcStr);	

	//cast int to QInt
	QInt& operator=(const int& x);
	
	//cast QInt to int (in case Qint is over the capability of int it may cause lost of data)
	int castToInt();
public:
	//set the bit at position i the value b
	//i=0 associate with LSB
	//i=127 associate with MSB
	void setBit(char i, bool b);

	//get the value of the bit at position i
	bool getBit(char i) const;

	//convert to 2's complement of this number
	void convertTo2sComplement();

	//return true if this Qfloat is zero, otherwise return false
	bool isZero()const;
public:
	//convert QInt to a string show Binary value
	//use for output in binary
	std::string DectoBin() const;

	//convert QInt to a string show Hexadecimal value
	//use for output in hexadecimal
	std::string DectoHex() const;

	//convert QInt to a string show Decimal value
	//use for output in decimal
	std::string DecToDecString()const;

	//convert the string inBinStr show binary value to this QInt
	//use for input from a binary string
	void BintoDec(std::string inBinStr);

	//convert the string inHexStr show hexadecimal value to this QInt
	//use for input from a hexadecimal string
	void HextoDec(std::string inHexStr);

	//convert the string inDecStr show decimal value to this QInt
	//use for input from a decimal string
	void DecStringToDec(std::string inDecStr);

	//use to convert binary string inBinStr to hexadecimal string outHexStr
	friend void BinToHex(std::string inBinStr, std::string outHexStr);

//input a QInt qi from istream istr
friend std::istream& operator>>(std::istream& istr, QInt& qi);

//output a QInt qi to the ostream ostr
friend std::ostream& operator<<(std::ostream& ostr, const QInt& qi);


	friend int GetRemainder(const QInt& x, const QInt& y);

	//plus x and y only between bit 'dau' and bit 'duoi' ('dau'<'duoi')
	static void Congtrongkhoang(QInt& x, QInt& y, int dau, int duoi);
	//minus x and y only between bit 'dau' and bit 'duoi' ('dau'<'duoi')
	static void Trutrongkhoang(QInt& x, QInt& y, int dau, int duoi);

	friend QInt operator+(const QInt &x, const QInt &y);
	friend QInt operator-(const QInt &x, const QInt &y);
	friend QInt operator*(const QInt &x, const QInt &y);
	friend QInt operator/(const QInt &x, const QInt &y);

	friend bool operator==(const QInt &x, const QInt &y);
	friend bool operator!=(const QInt &x, const QInt &y);
	friend bool operator<(const QInt &x, const QInt &y);	
	friend bool operator>(const QInt &x, const QInt &y);
	friend bool operator<=(const QInt &x, const QInt &y);
	friend bool operator>=(const QInt &x, const QInt &y);

	//x & y
	friend QInt operator&(const QInt &x, const QInt &y);
	//x | y
	friend QInt operator|(const QInt &x, const QInt &y);
	//x ^ y
	friend QInt operator^(const QInt &x, const QInt &y);
	//~x
	friend QInt operator~(const QInt &x);

	//shift left the variable x k bit
	friend QInt operator>>(const QInt &x, int k);
	//shift left the variable x k bit
	friend QInt operator<<(const QInt &x, int k);

	//rotate left the variable x k bit
	friend QInt rol(const QInt &x, int k);
	//rotate right the variable x k bit
	friend QInt ror(const QInt &x, int k);
};


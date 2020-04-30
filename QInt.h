#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "DecString.h"

class QInt
{
	static const int NUMBYTES = 16;
	static const int NUMBITS = 128;
	char bytes[NUMBYTES];
public:

	// Hiền

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
	QInt& operator=(const char* srcStr);	// str này ở hệ 10 thôi phải ko?
											// có nhận str kiểu như "0x12345ABC" hay "0b101110001" ko? CÓ

private:
	//set the bit at position i the value b
	//i=0 associate with LSB
	//i=127 associate with MSB
	void setBit(char i, bool b);

	//get the value of the bit at position i
	bool getBit(char i) const;

	//convert to 2's complement of this number
	void convertTo2sComplement();

	// cong 2 so x va y trong khoang tu bit dau den bit duoi + 1(dau<duoi), luu ket qua vao x
	// DE luu dau
	// phuc vu cho * /
	void Congtrongkhoang(QInt& x, QInt& y, int dau, int duoi);
	void Trutrongkhoang(QInt& x, QInt& y, int dau, int duoi);

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


//input a QInt qi from istream istr
friend std::istream& operator>>(std::istream& istr, QInt& qi);

//output a QInt qi to the ostream ostr
friend std::ostream& operator<<(std::ostream& ostr, const QInt& qi);

//////////////////////////////////////////////////
// Hiếu

friend QInt operator+(const QInt &x, const QInt &y);
friend QInt operator-(const QInt &x, const QInt &y);
friend QInt operator*(const QInt &x, const QInt &y);	// thuật toán Booth?
friend QInt operator/(const QInt &x, const QInt &y);	// KHÓ

//++, --, (int)-QInt, (int)+QInt

friend bool operator==(const QInt &x, const QInt &y);
friend bool operator!=(const QInt &x, const QInt &y);
friend bool operator<(const QInt &x, const QInt &y);	// chạy từ most significant bit tới
friend bool operator>(const QInt &x, const QInt &y);
friend bool operator<=(const QInt &x, const QInt &y);
friend bool operator>=(const QInt &x, const QInt &y);

//////////////////////////////////////////////////
// Tiến

friend QInt operator&(const QInt &x, const QInt &y);
friend QInt operator|(const QInt &x, const QInt &y);
friend QInt operator^(const QInt &x, const QInt &y);
friend QInt operator~(const QInt &x);

friend QInt operator>>(const QInt &x, int k);
friend QInt operator<<(const QInt &x, int k);

friend QInt rol(const QInt &x, int k);
friend QInt ror(const QInt &x, int k);
};

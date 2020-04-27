#pragma once
#include <iostream>
//#include "DecString.h"

class QInt
{
	static const int NUMBYTES = 16;
	static const int NUMBITS = 128;
	char bytes[NUMBYTES];
public:

	// Hiền

	QInt();
	QInt(const QInt& src);
	QInt(const char* srcStr);

	QInt& operator=(QInt& src);
	QInt& operator=(std::string srcStr);	// str này ở hệ 10 thôi phải ko?
											// có nhận str kiểu như "0x12345ABC" hay "0b101110001" ko? CÓ

private:
	void setBit(char i, bool b);
	bool getBit(char i) const;
	void convertTo2sComplement();
	void DectoBin(bool* &binStr) const;		// chỉ dùng để phục vụ hàm xuất
	void BintoDec(bool* &binStr);			// chỉ dùng để phục vụ hàm nhập từ chuỗi "0b010101"
	void DectoHex(char* &hexStr) const;
	void HextoDec(char* &hexStr);


friend std::istream& operator<<(std::istream& istr, QInt& qi);
friend std::ostream& operator>>(std::ostream& istr, const QInt& qi);

//////////////////////////////////////////////////
// Hiếu

friend QInt operator+(const QInt &x, const QInt &y);
friend QInt operator-(const QInt &x, const QInt &y);
friend QInt operator*(const QInt &x, const QInt &y);	// thuật toán Booth?
friend QInt operator/(const QInt &x, const QInt &y);	// KHÓ

// có viết mấy hàm +=, ++ đồ ko?	=> Ko

friend bool operator==(const QInt &x, const QInt &y);
friend bool operator!=(const QInt &x, const QInt &y);
friend bool operator<(const QInt &x, const QInt &y);	// chạy từ most significant bit tới
friend bool operator>(const QInt &x, const QInt &y);
friend bool operator<=(const QInt &x, const QInt &y);
friend bool operator>=(const QInt &x, const QInt &y);

//////////////////////////////////////////////////
// Tiến

friend QInt operator&(QInt &x, QInt &y);
friend QInt operator|(QInt &x, QInt &y);
friend QInt operator^(QInt &x, QInt &y);
friend QInt operator~(QInt &x);

friend QInt operator>>(QInt &x, int i);
friend QInt operator<<(QInt &x, int i);

friend QInt rol(QInt &x, int i);
friend QInt ror(QInt &x, int i);
};

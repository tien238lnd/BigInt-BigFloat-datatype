#pragma once
#include <iostream>
#include <string>

class QInt
{
	char Q[16];
public:
	QInt& operator=(QInt& des);
	QInt& operator=(std::string str);
	
friend std::istream& operator<<(std::istream& istr, QInt& qi);
friend std::ostream& operator>>(std::ostream& istr, QInt& qi);
	
friend bool* DectoBin(QInt x);
friend QInt BintoDec(bool* bit);

friend char* DectoHex(QInt x);
friend QInt HextoDec(char* hex);

friend QInt operator+(QInt &x, QInt &y);
friend QInt operator-(QInt &x, QInt &y);
friend QInt operator*(QInt &x, QInt &y);
friend QInt operator/(QInt &x, QInt &y);

// có viết mấy hàm +=, ++ đồ ko?	=> Ko

friend bool operator==(QInt &x, QInt &y);
friend bool operator!=(QInt &x, QInt &y);
friend bool operator<(QInt &x, QInt &y);
friend bool operator>(QInt &x, QInt &y);
friend bool operator<=(QInt &x, QInt &y);
friend bool operator>=(QInt &x, QInt &y);

friend QInt operator&(QInt &x, QInt &y);
friend QInt operator|(QInt &x, QInt &y);
friend QInt operator^(QInt &x, QInt &y);
friend QInt operator~(QInt &x);

friend QInt operator>>(QInt &x, int i);
friend QInt operator<<(QInt &x, int i);
friend QInt rol(QInt &x, int i);
friend QInt ror(QInt &x, int i);
};

char* BintoHex(bool* bit);
bool* HextoBin(char* hex);

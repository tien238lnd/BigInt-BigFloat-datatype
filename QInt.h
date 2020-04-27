#ifndef _QInt_h_
#define _QInt_h_

class DeQInt
{
private:
	static const int numbytes = 39;
	char bytes[numbytes];
public:
	DeQInt();
	DeQInt(const DeQInt&);
	DeQInt(const char*);

public:
	char getAt(char index);
	void setAt(char index, char value);
public:
	//DeQInt& operator=(const DeQInt& other);
	//DeQInt& operator=(const char* other);
public:
	bool checkNegative();
	bool checkZero();
	void convertToPositive();

	char devidedBy2();
	bool multipliedBy2();
	bool added1();
};


class QInt
{
private:
	static const int numbytes = 16;
	static const int numbits = 128;
	char bytes[numbytes];
public:
	QInt();
	QInt(const QInt&);
	QInt(const char*);
private:
	void setBit(char, char);
	char getBit(char);
public:
	void convertTo2sComplement();
	void DeQIntToQInt(const DeQInt&);
	void DecToQInt(const char*);
public:

};

#endif /*_QInt_h*/_
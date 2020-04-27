#include "QInt.h"

DeQInt::DeQInt()
{}
DeQInt::DeQInt(const DeQInt&)
{}
DeQInt::DeQInt(const char*)
{}
char DeQInt::getAt(char index)
{
	return 1;
}
void DeQInt::setAt(char index, char value)
{}


//DeQInt& DeQInt::operator=(const DeQInt& other); 
//DeQInt& DeQInt::operator=(const char* other);

bool DeQInt::checkNegative()
{
	return 1;
}
bool DeQInt::checkZero()
{
	return 1;
}
void DeQInt::convertToPositive()
{}

char DeQInt::devidedBy2()
{
	return 1;
}
bool DeQInt::multipliedBy2()
{
	return 1;
}
bool DeQInt::added1()
{
	return 1;
}


QInt::QInt()
{
}

QInt::QInt(const QInt& other)
{
	for (int i = 0; i < QInt::numbytes; i++)
	{
		this->bytes[i] = other.bytes[i];
	}
}

QInt::QInt(const char* other)
{
	//normalize and remove size
	DeQInt diTemp(other);		//normalize
	
	//do convert
	this->DeQIntToQInt(diTemp);
}

void QInt::setBit(char, char)
{

}
char QInt::getBit(char)
{
	return 1;
}

void QInt::convertTo2sComplement()
{

}

void QInt::DeQIntToQInt(const DeQInt& dqi)
{
	DeQInt dqiTemp = dqi;
	bool negativeFlag = false;

	if (dqiTemp.checkNegative())	//remove sign
	{
		negativeFlag = true;
		dqiTemp.convertToPositive();
	}

	//do convert
	int i = 0;
	while (!dqiTemp.checkZero())
	{
		char b = dqiTemp.devidedBy2();
		this->setBit(i, b);
	}

	//
	if (negativeFlag)
	{
		this->convertTo2sComplement();
	}
}

void QInt::DecToQInt(const char* dec)
{
	DeQInt dqiTemp(dec);

	this->DeQIntToQInt(dqiTemp);
}
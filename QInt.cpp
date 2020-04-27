#include "QInt.h"


void QInt::setBit(char i, bool b)
{
	if (b == 1)
		this->bytes[i / 16] = this->bytes[i / 16] | (1 << i % 16);
	else
		this->bytes[i / 16] = this->bytes[i / 16] & ~(1 << i % 16);
}


bool QInt::getBit(char i) const
{
	return (this->bytes[i / 16] >> i % 16) & 1;
}


void convertToComplement(QInt &x);

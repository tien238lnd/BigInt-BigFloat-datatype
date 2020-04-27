#include "QInt.h"

void setBit1(int &x, char i)	// LSB is 0
{
	x = x | (1 << i);
}

bool getBit(int x, char i)	// LSB is 0
{
	return (x >> i) & 1;
}

void convertToComplement(QInt &x);
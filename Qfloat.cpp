#include "Qfloat.h"


void Qfloat::setBit(char i, bool b)
{
	if (b == 1)
		this->bytes[i / 8] = this->bytes[i / 8] | (1 << i % 8);
	else
		this->bytes[i / 8] = this->bytes[i / 8] & ~(1 << i % 8);
}


bool Qfloat::getBit(char i) const
{
	return (this->bytes[i / 8] >> i % 8) & 1;
}

// void Qfloat::convertTo2sComplement()
// {
// 	//1's complement
// 	for (int i = 0; i < Qfloat::NUMBITS; i++)
// 	{
// 		this->setBit(i, 1 - this->getBit(i));
// 	}

// 	//add 1
// 	int i = 0;
// 	char remember = 1;
// 	do
// 	{
// 		char bitToSet = this->getBit(i) + remember;
// 		if (bitToSet < 2)
// 		{
// 			this->setBit(i, bitToSet);
// 			break;
// 		}
// 		else
// 		{
// 			this->setBit(i, 0);
// 		}

// 		i++;
// 	} while (i < Qfloat::NUMBITS);

// }
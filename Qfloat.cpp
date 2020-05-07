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

std::string Qfloat::toDecString() const
{
	// HARDCORE
	return std::string();
}

std::string Qfloat::toBinString() const
{
	std::string out;
	for (int i = Qfloat::NUMBITS - 1; i >= 0; i--)
		out += (this->getBit(i) + '0');
	return out;
}

void Qfloat::fromDecString(std::string src)
{
	// HARDCORE
}

void Qfloat::fromBinString(std::string src)
{
	int i = 0;
	for (; i < src.length(); i++)
	{
		//set value from input string to our Qfloat
		this->setBit(Qfloat::NUMBITS - 1 - i, src[i] - '0');
	}
	for (i = Qfloat::NUMBITS - 1 - i; i >= 0; i--)
	{
		//if the string has run out, just set bit to zero
		this->setBit(i, 0);
	}
}

Qfloat::Qfloat()
{
	memset(bytes, 0, Qfloat::NUMBYTES);
}

Qfloat::Qfloat(const Qfloat & src)
{
	for (int i = 0; i < Qfloat::NUMBITS; i++)
	{
		this->setBit(i, src.getBit(i));
	}
}

Qfloat::Qfloat(std::string src)
{
	if (src[0] == '0' && src[1] == 'b')
	{
		//delete "0b"
		src.erase(0, 2);
		
		this->fromBinString(src);
	}
	else
	{
		this->fromDecString(src);
	}
}


Qfloat::Qfloat(const char * src)
{
	*this = Qfloat(std::string(src));
}

Qfloat & Qfloat::operator=(const Qfloat & src)
{
	for (int i = 0; i < Qfloat::NUMBITS; i++)
	{
		this->setBit(i, src.getBit(i));
	}
	return *this;
}

Qfloat & Qfloat::operator=(std::string srcStr)
{
	*this = Qfloat(srcStr);

	return *this;
}

Qfloat & Qfloat::operator=(const char * srcStr)
{
	*this = Qfloat(std::string(srcStr));
	return *this;
}

std::ostream & operator<<(std::ostream & ostr, const Qfloat & qf)
{
	ostr << qf.toDecString();
	return ostr;
}

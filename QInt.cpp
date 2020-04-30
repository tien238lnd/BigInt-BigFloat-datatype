#include "QInt.h"

QInt::QInt()
{
	memset(bytes, 0, QInt::NUMBYTES);
}
QInt::QInt(const QInt& src)
{
	for (int i = 0; i < QInt::NUMBITS; i++)
	{
		this->setBit(i, src.getBit(i));
	}
}

QInt::QInt(std::string src)
{

	if (src[0] == '0' && src[1] == 'b')
	{
		//delete "0b"
		src.erase(0, 2);
		//
		this->BintoDec(src);
	}
	else if (src[0] == '0' && src[1] == 'x')
	{
		//delete "0x"
		src.erase(0, 2);
		//
		this->HextoDec(src);
	}
	else
	{
		this->DecStringToDec(src);
	}
	
}

QInt::QInt(const char* src)
{
	*this = QInt(std::string(src));
}

QInt& QInt::operator=(const QInt& src)
{
	for (int i = 0; i < QInt::NUMBITS; i++)
	{
		this->setBit(i, src.getBit(i));
	}

	return *this;
}

QInt& QInt::operator=(std::string srcStr)
{
	*this = QInt(srcStr);

	return *this;
}

QInt& QInt::operator=(const char* srcStr)
{
	*this = QInt(std::string(srcStr));
	return *this;
}

void QInt::setBit(char i, bool b)
{
	if (b == 1)
		this->bytes[i / 8] = this->bytes[i / 8] | (1 <<(i % 8));
	else
		this->bytes[i / 8] = this->bytes[i / 8] & ~(1 << (i % 8));
}


bool QInt::getBit(char i) const
{
	return  (this->bytes[i / 8] >> (i % 8) ) & 1;
}


void QInt::convertTo2sComplement()
{
	//1's complement
	for (int i = 0; i < QInt::NUMBITS; i++)
	{
		this->setBit(i, 1 - this->getBit(i));
	}

	//add 1
	int i = 0;
	char remember = 1;
	do
	{
		char bitToSet = this->getBit(i) + remember;
		if (bitToSet < 2)
		{
			this->setBit(i, bitToSet);
			break;
		}
		else
		{
			this->setBit(i, 0);
		}

		i++;
	} while (i < QInt::NUMBITS);

}


std::string QInt::DectoBin() const	// QInt to "101010" string, use for output function
{
	bool valueZoneFlag = false;
	std::string charDigit = " ";
	std::string outBinStr;

	for (int i = QInt::NUMBITS - 1; i >= 0; i--)
	{
		char nextBigDigit = this->getBit(i);

		if (nextBigDigit != 0) { valueZoneFlag = true; }

		if (valueZoneFlag)
		{
			//convert number digit to char digit
			charDigit[0] = '0' + nextBigDigit;
			//add to result hex string
			outBinStr += charDigit[0];
		}
	}
	return outBinStr;
}

std::string QInt::DectoHex() const
{
	bool valueZoneFlag = false;
	std::string charDigit = " ";
	std::string outHexStr;
	for (int i = QInt::NUMBITS-1; i>=0; i -= 4)
	{
		char nextBigDigit = 0;

		//get next four bits
		if(i<=8)
		{
			int k = 3;
		}
		for (int j = 0; j < 4; j++)
		{
			nextBigDigit <<= 1;
			nextBigDigit = nextBigDigit | char(this->getBit(i - j));
		}

		if (nextBigDigit != 0) { valueZoneFlag = true; }

		if (valueZoneFlag)
		{
			//convert number digit to char digit
			if (nextBigDigit <= 9) { charDigit[0] = '0' + nextBigDigit; }
			else { charDigit[0] = 'A' - 10 + nextBigDigit; }

			//add to result hex string
			outHexStr += charDigit[0];
		}

	}
	return outHexStr;
}

std::string QInt::DecToDecString()const
{
	QInt qiCopy(*this);
	int i = QInt::NUMBITS - 1;
	bool isNegative = false;
	if (qiCopy.getBit(127) == 1)
	{
		isNegative = true;
		qiCopy.convertTo2sComplement();
	}


	DecString decsrc;

	while (qiCopy.getBit(i) == 0) { i--; }

	while (i >= 0)
	{
		decsrc.multipliedBy2();
		if (qiCopy.getBit(i) == 1)
		{
			decsrc.added1();
		}

		i--;
	}

	std::string res;
	if (isNegative)
	{
		res = '-' + decsrc.getString();
	}
	else
	{
		res = decsrc.getString();
	}

	return res;
}

void QInt::BintoDec(std::string inBinStr)			//just use to convert a binary string "1010110100" to QInt
{
	char lastIndex = char(inBinStr.length() - 1);

	for (int i = 0; i < QInt::NUMBITS; i++)
	{
		if (i <= lastIndex)
		{
			//set last value of char input string to our QInt
			this->setBit(i, inBinStr[lastIndex - i] - '0');
		}
		else
		{
			//if the string has run out, just set bit to zero
			this->setBit(i, 0);
		}
	}
}

void QInt::HextoDec(std::string inHexStr)
{
	char lastIndex = char(inHexStr.length() - 1);
	
	for (int i = 0; i < QInt::NUMBITS; i+=4)
	{
		char nextSmallDigit = 0;

		//choose last valueable Hex digit and convert it to four bits in binary
		if (i / 4 <= lastIndex)
		{
			//choose last valueable Hex digit
			nextSmallDigit = inHexStr[lastIndex - i / 4];

			//convert it to four bits in binary, but save in a char with eight-bits (four last of eight bits)
			if (nextSmallDigit <= '9') { nextSmallDigit -= '0'; }
			else if (nextSmallDigit <= 'F') { nextSmallDigit -= 'A' - 10; }
			else { nextSmallDigit -= 'a' - 10; }
		}

		//set these four bit to the right place of our QInt
		for (int j = 0; j < 4; j++)
		{
			this->setBit(i + j, nextSmallDigit & 1);
			nextSmallDigit >>= 1;
		}
	}
}

void QInt::DecStringToDec(std::string inDecStr)
{
	bool isNegative = false;
	
	if (inDecStr[0] == '-')
	{
		isNegative = true;
		inDecStr.erase(0, 1);
	}

	//convert
	DecString decsrc = DecString(inDecStr);

	int i = 0;
	bool valuezone = true;
	while (i < QInt::NUMBITS)
	{
		if (valuezone)
		{
			if (decsrc.isZero()) { valuezone = false; }
			this->setBit(i, decsrc.dividedBy2());
		}
		else
		{
			this->setBit(i, 0);
		}
		i++;
	}

	//
	if (isNegative)
	{
		this->convertTo2sComplement();
	}

}


std::istream& operator>>(std::istream& istr, QInt& qi)
{
	char c;
	do
	{
		istr >> c;
	} while (c == ' ');

	std::string strqi;
	do
	{
		strqi += c;
		istr.get(c);
		
		//if (istr.) { break; }
		//istr >> c;
	} while (c == 'x' || c == 'b' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));

	qi = QInt(strqi);

	return istr;
}
std::ostream& operator<<(std::ostream& ostr, const QInt& qi)
{
	ostr << qi.DecToDecString();
	
	return ostr;
}

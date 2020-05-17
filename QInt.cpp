#include "QInt.h"


QInt::QInt()
{
	memset(bytes, 0, QInt::NUMBYTES);
}
QInt::QInt(const QInt& src):QInt()
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
QInt& QInt::operator=(const int& x)
{
	char* p = (char*)&x;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			this->setBit(i * 8 + j, *(p + i) >> j & 1);
		}
	}

	bool sign = this->getBit(31);
	for (int i = 32; i < QInt::NUMBITS; i++)
	{
		this->setBit(i, sign);
	}

	return *this;
}

int QInt::castToInt()
{
	int ires = 0;
	char* p = (char*)&ires;
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			*(p + i) |= this->getBit(i * 8 + j) << j;
		}
	}

	return ires;
}

void QInt::setBit(char i, bool b)
{
	if (b == 1)
		this->bytes[i / 8] = this->bytes[i / 8] | (1 << i % 8);
	else
		this->bytes[i / 8] = this->bytes[i / 8] & ~(1 << i % 8);
}


bool QInt::getBit(char i) const
{
	return (this->bytes[i / 8] >> i % 8) & 1;
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

bool QInt::isZero()const
{
	for (int i = 0; i < QInt::NUMBITS; i++)
	{
		if (getBit(i) != 0) { return false; }
	}

	return true;
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

	if (outBinStr.length() == 0) { outBinStr = "0"; }

	return outBinStr;
}

std::string QInt::DectoHex() const
{
	bool valueZoneFlag = false;
	std::string charDigit = " ";
	std::string outHexStr;
	for (int i = QInt::NUMBITS - 1; i >= 0; i -= 4)
	{
		char nextBigDigit = 0;

		//get next four bits
		if (i <= 8)
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

	if (outHexStr.length() == 0) { outHexStr = "0"; }

	return outHexStr;
}

std::string QInt::DecToDecString()const
{
	QInt qiCopy(*this);
	int i = QInt::NUMBITS - 1;
	bool isNegative = false;
	if (qiCopy.getBit(QInt::NUMBITS - 1) == 1)
	{
		isNegative = true;
		qiCopy.convertTo2sComplement();
	}

	QInt base_2[11] = { "0b0000", "0b0001","0b0010","0b0011","0b0100","0b0101","0b0110","0b0111","0b1000","0b1001", "0b1010" };
	QInt b10("0b1010");
	std::string res;

	int remainder = 0;
	while (!qiCopy.isZero())
	{
		remainder = (qiCopy % b10).castToInt();
		qiCopy = qiCopy / b10;
		char digit = remainder + '0';
		res = digit + res;
	}

	if (res.length() == 0) { res += '0'; }

	if (isNegative)
	{
		res = '-' + res;
	}

	return res;
}

void QInt::BintoDec(std::string inBinStr)			//just use to convert a binary string "1010110100" to QInt
{
	int lastIndex = int(inBinStr.length() - 1);

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
	int lastIndex = int(inHexStr.length() - 1);

	for (int i = 0; i < QInt::NUMBITS; i += 4)
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
	else if (inDecStr[0] == '+')
	{
		inDecStr.erase(0, 1);
	}
	
	//convert
	for (int i = 0; i < QInt::NUMBITS; i++)
	{
		this->setBit(i, 0);
	}

	QInt base_2[11] = { "0b0000", "0b0001","0b0010","0b0011","0b0100","0b0101","0b0110","0b0111","0b1000","0b1001", "0b1010" };
	QInt b("0b1");


	for (int i = inDecStr.length() - 1; i >= 0; i--)
	{
		if (inDecStr[i] < '0' || inDecStr[i]>'9') { break; }
		*this = *this + b * base_2[inDecStr[i] - '0'];
		b = b * base_2[10];
	}

	//
	if (isNegative)
	{
		this->convertTo2sComplement();
	}

}

//use to convert binary string inBinStr to hexadecimal string outHexStr
void BinToHex(std::string inBinStr, std::string outHexStr)
{
	outHexStr = QInt(inBinStr).DectoHex();
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

QInt operator+(const QInt& x, const QInt& y) {
	QInt kq;
	bool nho = 0;

	for (int i = 0; i < QInt::NUMBITS; i++)
	{
		int a = x.getBit(i) + y.getBit(i) + nho;
		if (a == 2) {
			kq.setBit(i, 0);
			nho = 1;
		}
		else if (a >= 2) {
			kq.setBit(i, 1);
			nho = 1;
		}
		else
		{
			kq.setBit(i, a);
			nho = 0;
		}
	}

	return kq;
}

QInt operator-(const QInt& x, const QInt& y) {
	QInt kq;

	bool nho = 0;
	for (int i = 0; i < QInt::NUMBITS; i++)
	{
		bool a = x.getBit(i);
		bool b = y.getBit(i);
		if (a - b - nho == -1) {
			kq.setBit(i, 1);
			nho = 1;
		}
		else if (a - b - nho < -1) {
			kq.setBit(i, 0);
			nho = 1;
		}
		else {
			kq.setBit(i, a - b - nho);
			nho = 0;
		}
	}

	return kq;
}

void QInt::Congtrongkhoang(QInt& x, QInt& y, int dau, int duoi) {
	bool nho = 0;
	for (int i = dau; i <= duoi; i++)
	{
		short int a = x.getBit(i) + y.getBit(i);
		if (a + nho == 2) {
			x.setBit(i, 0);
			nho = 1;
		}
		else if (a + nho >= 2) {
			x.setBit(i, 1);
			nho = 1;
		}
		else
		{
			x.setBit(i, a + nho);
			nho = 0;
		}
	}
}

void QInt::Trutrongkhoang(QInt& x, QInt& y, int dau, int duoi) {
	bool nho = 0;
	for (int i = dau; i <= duoi; i++)
	{
		bool a = x.getBit(i);
		bool b = y.getBit(i);
		if (a - b - nho == -1) {
			x.setBit(i, 1);
			nho = 1;
		}
		else if (a - b - nho < -1) {
			x.setBit(i, 0);
			nho = 1;
		}
		else {
			x.setBit(i, a - b - nho);
			nho = 0;
		}
	}
}

QInt operator*(const QInt& x, const QInt& y)
{
	//booth
	QInt res[2] = { "0b0", "0b0" };
	bool pre_bit = 0;
	int numbit2qint = 2 * QInt::NUMBITS;

	int most_bit = QInt::NUMBITS - 2;
	for(; most_bit>=0 && y.getBit(most_bit) == y.getBit(most_bit + 1); most_bit--){}
	if (most_bit < QInt::NUMBITS - 1) { most_bit++; }

	for (int i = 0; i <= most_bit; i++)
	{
		bool this_bit = y.getBit(i);
		if (this_bit == 1 && pre_bit==0)
		{
			//subtract
			char remember = 0;
			char tmp = 0;
			for (int j = 0; i + j < numbit2qint; j++)
			{
				char bitthisj = x.getBit(QInt::NUMBITS - 1);//sign
				if (j < QInt::NUMBITS) { bitthisj = x.getBit(j); }

				tmp = res[(j + i) / QInt::NUMBITS].getBit((j + i) % QInt::NUMBITS) - char(remember + bitthisj);
				if (tmp < 0)
				{
					tmp += 2;
					remember = 1;
				}
				else { remember = 0; }
				res[(j + i) / QInt::NUMBITS].setBit((j + i) % QInt::NUMBITS, tmp);
			}
		}
		else if (this_bit == 0 && pre_bit == 1)
		{
			//add
			char remember = 0;
			char tmp = 0;
			for (int j = 0; i + j < numbit2qint; j++)
			{
				char bitthisj = x.getBit(QInt::NUMBITS - 1);//sign
				if (j < QInt::NUMBITS) { bitthisj = x.getBit(j); }

				tmp = res[(j + i) / QInt::NUMBITS].getBit((j + i) % QInt::NUMBITS) + char(remember + bitthisj);
				if (tmp > 1)
				{
					tmp -= 2;
					remember = 1;
				}
				else { remember = 0; }
				res[(j + i) / QInt::NUMBITS].setBit((j + i) % QInt::NUMBITS, tmp);
			}
		}

		pre_bit = this_bit;
	}

	return res[0];
}

QInt operator/(const QInt& x, const QInt& y)
{
	QInt divisor = y;
	QInt dividend[2] = { x, "0b0" };
	QInt res("0b0");

	bool res_is_neg = false;
	if (x.getBit(QInt::NUMBITS - 1) == 1) { dividend[0].convertTo2sComplement(); }
	if (y.getBit(QInt::NUMBITS - 1) == 1) { divisor.convertTo2sComplement(); }
	if (x.getBit(QInt::NUMBITS - 1) != y.getBit(QInt::NUMBITS - 1)) { res_is_neg = true; }
	
	for (int i = QInt::NUMBITS - 1; i >= 0; i--)
	{
		//found which is bigger
		int dividend_vs_divisor = 0;
		for (int j = QInt::NUMBITS - 1; j >= 0; j--)
		{
			bool ddbit = dividend[(i + j) / QInt::NUMBITS].getBit((i + j) % QInt::NUMBITS);
			bool dsbit = divisor.getBit(j);

			if (ddbit > dsbit) { dividend_vs_divisor = 1; break; }
			else if (ddbit < dsbit) { dividend_vs_divisor = -1; break; }
		}

		if (dividend_vs_divisor >= 0)//dividend bigger than divisor
		{
			//set res bit to 1
			res.setBit(i, 1);
			//subtract
			char remember = 0;
			char tmp = 0;
			for (int j = 0; j < QInt::NUMBITS; j++)//we can sure the result of substract always positive, so we dont need to run till end of dividend
			{
				tmp = dividend[(j + i) / QInt::NUMBITS].getBit((j + i) % QInt::NUMBITS) - char(remember + divisor.getBit(j));
				if (tmp < 0)
				{
					tmp += 2;
					remember = 1;
				}
				else { remember = 0; }
				dividend[(j + i) / QInt::NUMBITS].setBit((j + i) % QInt::NUMBITS, tmp);
			}
		}
	}
	
	if (res_is_neg == true)
	{
		res.convertTo2sComplement();
	}

	return res;
}

QInt operator%(const QInt& x, const QInt& y)
{
	QInt divisor = y;
	QInt dividend[2] = { x, "0b0" };
	QInt res("0b0");

	bool res_is_neg = false;
	if (x.getBit(QInt::NUMBITS - 1) == 1) { dividend[0].convertTo2sComplement(); }
	if (y.getBit(QInt::NUMBITS - 1) == 1) { divisor.convertTo2sComplement(); }
	if (x.getBit(QInt::NUMBITS - 1) != y.getBit(QInt::NUMBITS - 1)) { res_is_neg = true; }

	for (int i = QInt::NUMBITS - 1; i >= 0; i--)
	{
		//found which is bigger
		int dividend_vs_divisor = 0;
		for (int j = QInt::NUMBITS - 1; j >= 0; j--)
		{
			bool ddbit = dividend[(i + j) / QInt::NUMBITS].getBit((i + j) % QInt::NUMBITS);
			bool dsbit = divisor.getBit(j);

			if (ddbit > dsbit) { dividend_vs_divisor = 1; break; }
			else if (ddbit < dsbit) { dividend_vs_divisor = -1; break; }
		}

		if (dividend_vs_divisor >= 0)//dividend bigger than divisor
		{
			//set res bit to 1
			res.setBit(i, 1);
			//subtract
			char remember = 0;
			char tmp = 0;
			for (int j = 0; j < QInt::NUMBITS; j++)//we can sure the result of substract always positive, so we dont need to run till end of dividend
			{
				tmp = dividend[(j + i) / QInt::NUMBITS].getBit((j + i) % QInt::NUMBITS) - char(remember + divisor.getBit(j));
				if (tmp < 0)
				{
					tmp += 2;
					remember = 1;
				}
				else { remember = 0; }
				dividend[(j + i) / QInt::NUMBITS].setBit((j + i) % QInt::NUMBITS, tmp);
			}
		}
	}

	return dividend[0];
}

bool operator==(const QInt& x, const QInt& y) {
	for (unsigned char i = 0; i < QInt::NUMBITS; i++)
	{
		if (x.getBit(i) != y.getBit(i))
		{
			return false;
		}

	}
	return true;

}

bool operator!=(const QInt& x, const QInt& y) {
	
	return !(x==y);
}

bool operator>(const QInt& x, const QInt& y) {

	bool a = x.getBit(QInt::NUMBITS - 1);
	bool b = y.getBit(QInt::NUMBITS - 1);

	if (a == 1 && b == 0) {
		return false;
	}
	else if (a == 0 && b == 1) {
		return true;
	}
	else {

		for (char i = QInt::NUMBITS - 2; i >= 0; i--)
		{
			if (x.getBit(i) > y.getBit(i))
			{
				return true;
			}
			else if (x.getBit(i) < y.getBit(i)) {
				return false;
			}
		}
		// xuong toi day nghia la bang nhau
		return false;

	}
}

bool operator<(const QInt& x, const QInt& y) {

	bool a = x.getBit(QInt::NUMBITS - 1);
	bool b = y.getBit(QInt::NUMBITS - 1);

	if (a == 1 && b == 0) {
		return true;
	}
	else if (a == 0 && b == 1) {
		return false;
	}
	else {

		for (char i = QInt::NUMBITS - 2; i >= 0; i--)
		{
			if (x.getBit(i) < y.getBit(i))
			{
				return true;
			}
			else if (x.getBit(i) > y.getBit(i)) {
				return false;
			}
		}
		// xuong toi day nghia la bang nhau
		return false;

	}
}

bool operator<=(const QInt& x, const QInt& y) {
	if (x > y) {
		return false;
	}
	return true;
}

bool operator>=(const QInt& x, const QInt& y) {
	if (x < y) {
		return false;
	}
	return true;
}

QInt operator&(const QInt & x, const QInt & y)
{
	QInt ans;
	for (int i = 0; i < QInt::NUMBITS; i++)
		ans.setBit(i, x.getBit(i) & y.getBit(i));
	return ans;
}

QInt operator|(const QInt & x,const QInt & y)
{
	QInt ans;
	for (int i = 0; i < QInt::NUMBITS; i++)
		ans.setBit(i, x.getBit(i) | y.getBit(i));
	return ans;
}

QInt operator^(const QInt & x,const QInt & y)
{
	QInt ans;
	for (int i = 0; i < QInt::NUMBITS; i++)
		ans.setBit(i, x.getBit(i) ^ y.getBit(i));
	return ans;
}

QInt operator~(const QInt & x)

{
	QInt ans;
	for (int i = 0; i < QInt::NUMBITS; i++)
		ans.setBit(i, !(x.getBit(i)));
	return ans;
}

QInt operator>>(const QInt & x, int k)	// cài đặt bên trong là << - left shift, lấy bit dấu tận cùng bên phải fill vào
{
	k = k % QInt::NUMBITS;
	if (k == 0)
		return x;
	QInt ans;
	for (int i = 0; i < QInt::NUMBITS - 1 - k; i++)
		ans.setBit(i, x.getBit(i + k));
	for (int i = QInt::NUMBITS - 1 - k; i < QInt::NUMBITS; i++)
		ans.setBit(i, x.getBit(QInt::NUMBITS - 1));
	return ans;
}

QInt operator<<(const QInt & x, int k)	// cài đặt bên trong là >> - right shift, lấy bit 0 fill vào
{
	k = k % QInt::NUMBITS;
	if (k == 0)
		return x;
	QInt ans;
	ans.setBit(QInt::NUMBITS - 1, x.getBit(QInt::NUMBITS - 1));
	for (int i = QInt::NUMBITS - 2; i >= k; i--)
		ans.setBit(i, x.getBit(i - k));
	for (int i = k - 1; i >= 0; i--)
		ans.setBit(i, 0);
	return ans;
}

QInt rol(const QInt & x, int k)	// cài đặt bên trong là xoay phải
{
	k = k % QInt::NUMBITS;
	if (k == 0)
		return x;
	QInt ans;
	for (int i = 0; i < k; i++)
		ans.setBit(i, x.getBit(QInt::NUMBITS + i - k));
	for (int i = k; i < QInt::NUMBITS; i++)
		ans.setBit(i, x.getBit(i - k));
	return ans;
}

QInt ror(const QInt & x, int k)	// cài đặt bên trong là xoay trái
{
	k = k % QInt::NUMBITS;
	if (k == 0)
		return x;
	QInt ans;
	for (int i = 0; i < QInt::NUMBITS - k; i++)
		ans.setBit(i, x.getBit(i + k));
	for (int i = QInt::NUMBITS - k; i < QInt::NUMBITS; i++)
		ans.setBit(i, x.getBit(i + k - QInt::NUMBITS));
	return ans;
}



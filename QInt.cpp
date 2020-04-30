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

/////////////////////////////
////////**************///////
/////////////////////////////
QInt operator+(const QInt& x, const QInt& y) {
	QInt kq;
	for (int i = 0; i < QInt::NUMBITS; i++)
	{
		bool nho = 0;
		int a = x.getBit(i) + y.getBit(i);
		if (a + nho == 2) {
			kq.setBit(i, 0);
			nho = 1;
		}
		else if (a + nho >= 2) {
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
	for (int i = 0; i < QInt::NUMBITS; i++)
	{
		bool nho = 0;
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

QInt operator*(const QInt& x, const QInt& y) {

	QInt kq = y;
	// mask dong vai tro nhu M trong thuat toan
	QInt mask = x;
	bool q_1 = 0; // dong vai tro q-1 
	// dung de xet dau q1, xetdauq2
	bool dauq1 = x.getBit(QInt::NUMBITS - 1);
	bool dauq2 = y.getBit(QInt::NUMBITS - 1);

	// so bit cua multifier sau khi chuan hoa
	unsigned int count = 0;

	//chuan hoa so kq (boi vi dang truoc co the toan la so 1 hoac 0)
	for (int i = QInt::NUMBITS - 1; i >= 0; i--)
	{
		bool a = kq.getBit(i);
		bool b = kq.getBit(char(i - 1));
		if (a && b)
		{
			if (a)
			{
				kq.setBit(i, 0);
			}
		}
		else
		{
			if (a) { count = i + 1; }
			count = i;
			break;
		}
	}

	//chuan hoa so bi chia (boi vi dang truoc co the toan la so 1 hoac 0)
	for (char i = QInt::NUMBITS - 1; i >= 0; i--)
	{
		bool a = mask.getBit(i);
		bool b = mask.getBit(i - 1);
		if (a && b)
		{
			if (a)
			{
				mask.setBit(i, 0);
			}
		}
		else {
			break;
		}
	}
	// mask co dang 00000multifier00000 (khuc sau co so 0 bang so bits multifier)
	mask << (count);

	for (unsigned char i = 0; i < count; i++)
	{
		if (q_1 == 0 && kq.getBit(0) == 1) {
			kq = kq - mask;
		}
		else if (q_1 == 1 && kq.getBit(0) == 0)
		{
			kq = kq + mask;
		}
		q_1 = kq.getBit(0);
		kq >> 1;

	}


	// luc nay ket qua van co the sai, vi can phai dien bit trong vao truoc do nua
	// Neu nhu 2 so khac dau thi dien bit 1 vao truoc do
	if (dauq1 && dauq2 == false) {
		for (char i = QInt::NUMBITS - 1; i >= 0; i--)
		{
			if (kq.getBit(i) == false) {
				kq.setBit(i, 1);
			}
			else { break; }
		}

	}
	return kq;

}

QInt operator/(const QInt& x, const QInt& y) {

	QInt sobichia = x;
	QInt sochia = y;

	//neu co so am -> chuyen ve so khong am
	bool dau1 = sobichia.getBit(QInt::NUMBITS - 1);
	bool dau2 = sochia.getBit(QInt::NUMBITS - 1);
	if (!dau1) {
		sobichia.convertTo2sComplement();
	}
	if (!dau2)
	{
		sochia.convertTo2sComplement();
	}


	// Danh dau do dai cua 2 so chia va bi chia
	//vd : mark = 5 -> car so chia va so bi chia co do dai la 6 bits
	unsigned int mark;

	// vua xet xem so bi chia co lon hon so chia khong ? vua xet do dai
	for (char i = QInt::NUMBITS - 1; i >= 0; i--)
	{
		bool a = sobichia.getBit(i);
		bool b = sochia.getBit(i);

		// Neu nhu so bi chia be hon so chia
		if (a == 0 && b == 1)
		{
			// tra ve so Qint 0
			return QInt("0");
		}
		else if (a == 1) {
			mark = i;
		}
	}

	// mark se co dang sochia00000 (so luong bit 0 o phia sau bang so bit cua sobichia)
	QInt mask = sochia;
	mask << mark;

	// dung de lap
	unsigned int count = mark;

	for (int i = count; i >= 1; i--)
	{
		sobichia << 1;
		sobichia = sobichia - mask;

		// kiem tra xem kq co am khong
		if (sobichia.getBit(QInt::NUMBITS - 1))
		{
			sobichia.setBit(0, 0);
			sobichia = sobichia + mask;

		}
		else
		{
			sobichia.setBit(0, 1);
		}
	}

	// Neu lay phan sobichia & 0000011111 (so luong bit 1 o phia sau bang so bit cua sobichia, so luong bit 0 bang mark)
	for (unsigned char i = QInt::NUMBITS - 1; i > mark; i--)
	{
		sobichia.setBit(i, 0);
	}

	// neu 2 so am thi doi dau
	if (!(dau1 && dau2)) {
		sobichia.convertTo2sComplement();
	}

	return sobichia;

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
	for (unsigned char i = 0; i < QInt::NUMBITS; i++)
	{
		if (x.getBit(i) == y.getBit(i))
		{
			return false;
		}

	}
	return true;

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

/////////////////////////////
////////**************///////
/////////////////////////////


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
	for (int i = QInt::NUMBITS - 1; i >= k; i--)
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
	return QInt();
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
	return QInt();
}



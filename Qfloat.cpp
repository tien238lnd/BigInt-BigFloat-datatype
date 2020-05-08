#include "Qfloat.h"

Qfloat _0 = "0b0";
Qfloat _1 = "0b0011111111111111";
Qfloat _2 = "0b0100000000000000";
Qfloat _3 = "0b01000000000000001";
Qfloat _4 = "0b0100000000000001";
Qfloat _5 = "0b010000000000000101";
Qfloat _6 = "0b01000000000000011";
Qfloat _7 = "0b010000000000000111";
Qfloat _8 = "0b0100000000000010";
Qfloat _9 = "0b0100000000000010001";
Qfloat _10 = "0b010000000000001001";
Qfloat BCD[11] = { _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10 };

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
	for (int i = 127; i >= 0; i--)
		out += (this->getBit(i) + '0');
	return out;
}

void cut_off_meaningful_digits(std::string &str)
{
	if (str.length() > 33)
		str.erase(33);
}

Qfloat calculate_from_integral_part(const std::string &src, int exponent = 0)	// exponent if pass must > 0
{
	Qfloat result;
	for (int i = 0; i < src.length(); i++)
	{
		if (src[i] != '0')
			result = result * _10 + BCD[src[i] - '0'];
		else
			result = result * _10;
	}
	while (exponent-- > 0)
		result = result * _10;
	return result;
}

Qfloat calculate_from_fraction_part(const std::string &src, int exponent = 0)	// // exponent if pass must < 0
{
	Qfloat result;
	for (int i = src.length() - 1; i >= 0; i--)
	{
		if (src[i] != '0')
			result = (result + BCD[src[i] - '0']) / _10;
		else
			result = result / _10;
	}
	while (exponent++ < 0)
		result = result / _10;
	return result;
}

int move_floating_point_based_on_exponent(std::string &str, int &exponent) // exponent >= 0
{
	// 1.2345e0  --> 1.2345
	// 1.2345e3  --> 1234.5
	// 1.2345e10 --> 12345e6
	// 1.2345e4  --> 12345
	int point_locate = 1;
	while (exponent != 0 && point_locate != str.length() - 1)
	{
		exponent--;
		std::swap(str[point_locate], str[point_locate + 1]);
		point_locate++;
	}
	return point_locate;
}

void Qfloat::fromDecString(std::string src)
{
	// HARDCORE
	
	// PREPROCESSING
	// B1: Sign
	if (src[0] == '-')
	{
		this->setBit(127, 1);
		src.erase(0, 1);
	}
	// B2: Exponent
	int exp_locate = src.find('e');
	if (exp_locate == std::string::npos)
	{
		exp_locate = src.find('E');
		if (exp_locate == std::string::npos)
			exp_locate = -1;
	}
	if (exp_locate == -1)
	{
		// viet theo kieu bth => lay phan tri cat ngang 33 digits, roi tach phan nguyen va phan phay rieng de xu ly
		cut_off_meaningful_digits(src);

		if (src.back() == '.')				// 120.
		{
			src.pop_back();
			*this = calculate_from_integral_part(src);
		}
		else if (src[0] == '.')				// .12
		{
			*this = calculate_from_fraction_part(src);
		}
		else
		{
			int point_locate = src.find('.');
			if (point_locate == std::string::npos)	// 120
			{
				*this = calculate_from_integral_part(src);
			}
			else									// 12.12
			{
				std::string integral_part = src.substr(0, point_locate);
				std::string fractional_part = src.substr(point_locate + 1);
				*this = calculate_from_integral_part(integral_part) + calculate_from_fraction_part(fractional_part);
			}
		}
		return;
	}
	else
	{
		// viet theo kieu khoa hoc => tach rieng phan tri va phan mu
		int exponent = std::stoi(src.substr(exp_locate + 1));
		src.erase(exp_locate);
		cut_off_meaningful_digits(src);
		if (exponent >= 0)
		{
			if (src.length() == 1)		// 2e678
			{
				*this = calculate_from_integral_part(src, exponent);
				return;
			}
										// 2.345e678
			int point_locate = move_floating_point_based_on_exponent(src, exponent);
			if (point_locate == src.length() - 1)
			{
				src.pop_back();
				*this = calculate_from_integral_part(src, exponent);
				return;
			}
			else
			{
				std::string integral_part = src.substr(0, point_locate);
				std::string fractional_part = src.substr(point_locate + 1);
				*this = calculate_from_integral_part(integral_part) + calculate_from_fraction_part(fractional_part);
				return;
			}
		}
		else
		{
			src.erase(1, 1); exponent++;
			*this = calculate_from_fraction_part(src, exponent);
			return;
		}
	}

}

void Qfloat::fromBinString(std::string src)
{
	int i = 0;
	for (; i < src.length(); i++)
	{
		//set value from input string to our Qfloat
		this->setBit(127 - i, src[i] - '0');
	}
	for (i = 127 - i; i >= 0; i--)
	{
		//if the string has run out, just set bit to zero
		this->setBit(i, 0);
	}
}

Qfloat::Qfloat()
{
	memset(bytes, 0, 16);
}

Qfloat::Qfloat(const Qfloat & src)
{
	for (int i = 0; i < 128; i++)
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
	for (int i = 0; i < 128; i++)
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

void Qfloat::addexponent(int exp)
{
	char* thisexp = &(this->bytes[Qfloat::NUMBYTES - 2]);
	char remember = 0;
	int und = Qfloat::NUMBYTES - 16;
	for (int i = 0; i < 15; i++)
	{
		//get bit;
		char bitiexp = (exp >> i) & 1;
		char bitithis = getBit(und + i);

		//delete bit
		exp &= ~(1 << i);

		//set bit
		setBit(und + i, (remember + bitiexp + bitithis) % 2);
		remember = (remember + bitiexp + bitithis) / 2;
	}

	if (remember == 1 || exp != 0)//overflow exponent
	{
		for (int i = 0; i < 15; i++)
		{
			setBit(und + i, 1);
		}

		//not enough, the significant also must set to all zero ->infinity
		for (int i = 0; i < und; i++)
		{
			setBit(i, 0);
		}
	}
}

Qfloat operator+(const Qfloat & x, const Qfloat & y)
{
	return Qfloat();
}

Qfloat operator-(const Qfloat & x, const Qfloat & y)
{
	return Qfloat();
}

Qfloat operator*(const Qfloat & x, const Qfloat & y)
{
	return Qfloat();
}

Qfloat operator/(const Qfloat & x, const Qfloat & y)
{
	return Qfloat();
}

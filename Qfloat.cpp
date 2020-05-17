#include "QFloat.h"
#include "QInt.h"

QFloat _0 = "0b0";
QFloat _1 = "0b0011111111111111";
QFloat _2 = "0b0100000000000000";
QFloat _3 = "0b01000000000000001";
QFloat _4 = "0b0100000000000001";
QFloat _5 = "0b010000000000000101";
QFloat _6 = "0b01000000000000011";
QFloat _7 = "0b010000000000000111";
QFloat _8 = "0b0100000000000010";
QFloat _9 = "0b0100000000000010001";
QFloat _10 = "0b010000000000001001";
QFloat _BCD[11] = { _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10 };

QFloat _pINF = "0b0111111111111111";

QFloat _nINF = "0b1111111111111111";

QFloat _NaN = "0b0111111111111111111";

void QFloat::setBit(char i, bool b)
{
	if (b == 1)
		this->bytes[i / 8] = this->bytes[i / 8] | (1 << i % 8);
	else
		this->bytes[i / 8] = this->bytes[i / 8] & ~(1 << i % 8);
}


bool QFloat::getBit(char i) const
{
	return (this->bytes[i / 8] >> i % 8) & 1;
}

// ham bo tro 

// so sanh 2 mu cua 2 so QFloat, tra ve do chech lech 
// Neu x > y tra ve so duong
// Neu x < y tra ve so am
int CompareExponent(const QFloat& x, const QFloat& y) {

	int ans = 0;
	int pow2x = 1;

	bool nho = 0;
	for (int i = 112; i <= 126; i++)
	{
		bool a = x.getBit(i);
		bool b = y.getBit(i);
		if (a - b - nho == -1) {
			ans = ans + pow2x;
			nho = 1;
		}
		else if (a - b - nho < -1) {
			nho = 1;
		}
		else {
			ans = ans + pow2x * (a - b - nho);
			nho = 0;
		}
		pow2x = pow2x * 2;
	}

	ans += -1 * nho * pow2x;

	return ans;
}

int QFloat::get_exponent() const // of 2, not 10
{
	int res = 0;
	char* pres = (char*)&res;
	*pres = this->bytes[14];
	pres += 1;
	*pres = this->bytes[15];

	res &= ~(1 << 15);

	res = res - QFloat::BIAS;
	return res;

}

char CheckTypeofQFloat(const QFloat& q) {
	short tongmu = 0, tongsignicand = 0;
	for (int i = 126; i >= 112; i--)
	{
		tongmu += q.getBit(i);
	}

	for (int i = 111; i >= 0; i--)
	{
		tongsignicand += q.getBit(i);
	}

	if (tongmu + tongsignicand == 0) {
		return QFloat::Zero;
	}
	else if (tongmu == 0 && tongsignicand != 0) {
		return QFloat::Denormalize;
	}
	else if (tongmu == 15 && tongsignicand == 0) {
		return QFloat::Infinity;
	}
	else if (tongmu == 15 && tongsignicand != 0) {
		return QFloat::Nan;
	}
	else {
		return QFloat::Normal;
	}

}

void QFloat::modf(QFloat& integral, QFloat& fractional) const
{
	integral = fractional = _0;
	int exp = this->get_exponent();
	if (exp < 0)
	{
		integral = _0;
		fractional = *this;
	}
	else if (exp >= 112)
	{
		integral = *this;
		fractional = _0;
	}
	else
	{
		int o = 126;	// index run on the bit-chain of original float
		// Exponent of integral part is the same as the original
		for (; o > 111; o--)
		{
			integral.setBit(o, this->getBit(o));
		}
		// Significant of integral part is first (exp) bits of that in the original 
		for (; o > 111 - exp; o--)
		{
			integral.setBit(o, this->getBit(o));
		}
		// Exponent of fraction part is number of bit from (111-exp) to first next bit 1
		int exp_frac = -1;	// 1 bit default of normalized form
		while (o >= 0 && this->getBit(o--) == 0)
		{
			exp_frac--;
		}
		if (this->getBit(o + 1) == 0)
		{
			fractional = _0;
		}
		else
		{
			unsigned exp_frac_bias = exp_frac + 16383;	// bias representation
			int f = 126;	// index run on the bit-chain of the fractional float
			for (int i = 14; f > 111; f--, i--)
			{
				fractional.setBit(f, (exp_frac_bias >> i & 1));
			}

			int save_o = o;
			// Significant of fraction part is from that first bit 1 to end in the original
			for (; o >= 0; o--, f--)
			{
				fractional.setBit(f, this->getBit(o));
			}

			//found pattern
			int num_type_of_pattern = (save_o >= 4) * save_o / 4;
			int* pattern_have_i_bit = new int[num_type_of_pattern + 1];
			for (int i = 1; i <= num_type_of_pattern; i++) { pattern_have_i_bit[i] = 0; }

			for (int id = 0; id <= save_o; id++)
			{
				for (int i = 1; i <= num_type_of_pattern; i++)
				{
					if (this->getBit(id) != this->getBit(id % i))
					{
						if (pattern_have_i_bit[i] == id / i) { pattern_have_i_bit[i]--; }
					}
					else
					{
						if (id % i == i - 1 && pattern_have_i_bit[i] == id / i)
						{
							pattern_have_i_bit[i]++;
						}
					}
				}
			}

			int i = num_type_of_pattern;
			for (; i >= 1; i--)
			{
				if (pattern_have_i_bit[i] >= 2)
				{
					break;
				}
			}

			//if found pattern
			if (i != 0)
			{
				int id = i - 1;
				while (f >= 0)
				{
					fractional.setBit(f, this->getBit(id));
					id--;
					f--;
					if (id < 0) { id = i - 1; }
				}
			}
			delete[]pattern_have_i_bit;
		}
	}
}

char QFloat::toChar() const
{
	int bit_different[10] = { 0 };
	for (int b = 127; b >= 109; b--)
	{
		bool bval = this->getBit(b);
		for (int i = 0; i < 10; i++)
		{
			if (bit_different[i] != 0 || bval != _BCD[i].getBit(b))
			{
				bit_different[i]++;
			}
		}
	}

	int id = 0;
	int min = 20;
	for (int k = 0; k < 10; k++)
	{
		if (min > bit_different[k]) { min = bit_different[k]; id = k; }
	}

	return id + '0';
}

std::string print_from_integral_part(QFloat& src, int& exponent)
{
	// exponent = 0;
	QFloat digit_extracted;	// in form of 0.x
	std::string result;
	// src is alway integral round
	// recognize zero chain from point, go left to meet the first meaningful digit
	while (true)
	{
		QFloat integral_with_1_digit_after_point = src / _10;
		integral_with_1_digit_after_point.modf(src, digit_extracted);
		if (CheckTypeofQFloat(digit_extracted) == QFloat::Zero)
			exponent++;
		else
		{
			break;
		}
	}
	while (true) {
		digit_extracted = digit_extracted * _10; // 0.3 -> 3
		result = digit_extracted.toChar() + result;
		if (CheckTypeofQFloat(src) == QFloat::Zero)
			break;
		QFloat integral_with_1_digit_after_point = src / _10;
		integral_with_1_digit_after_point.modf(src, digit_extracted);
	}
	return result;
}

std::string print_from_fractional_part(QFloat& src, int& exponent, int digits_in_integral_part = 0)
{
	QFloat digit_extracted;	// in form of x
	std::string result;
	// src is alway a fraction less than 1 : 0.xxx...
	// recognize zero chain from point, go right to meet the first meaningful digit
	while (true)
	{
		QFloat fractional_with_1_digit_before_point = src * _10;
		fractional_with_1_digit_before_point.modf(digit_extracted, src);
		if (CheckTypeofQFloat(digit_extracted) == QFloat::Zero)
			exponent--;
		else break;
	}
	while (true) {
		result = result + digit_extracted.toChar();
		if (CheckTypeofQFloat(src) == QFloat::Zero || (int)result.length() > (32 - (digits_in_integral_part != 0) * (digits_in_integral_part - exponent - 1)))
			break;
		QFloat fractional_with_1_digit_before_point = src * _10;
		fractional_with_1_digit_before_point.modf(digit_extracted, src);
	}
	while (result.back() == '0')
		result.pop_back();
	return result;
}

std::string QFloat::toDecString() const
{
	// HARDCORE
	char type = CheckTypeofQFloat(*this);
	switch (type)
	{
	case QFloat::Zero :
		return "0";
	case QFloat::Infinity:
		if (this->getBit(127) == 0)
			return "+inf";
		else
			return "-inf";
	case QFloat::Nan :
		return "NaN";
	}
	

	QFloat integral, fractional;
	this->modf(integral, fractional);
	std::string result;
	// chia 2 th: e >= 0 và e < 0, dựa vào integral >= 1 hay ko
	// integral = 1 thì e=0, > 1 thì e > 0, =0 thì e <0
	if (CheckTypeofQFloat(fractional) == QFloat::Zero) // only integral part
	{
		int exponent = 0;
		result = print_from_integral_part(integral, exponent);
		if (exponent + result.length() > 6)
		{
			result += "e" + std::to_string(exponent + result.length() - 1);
			if (result[1] != 'e')
				result.insert(1, ".");
		}
		else
		{
			while (exponent-- > 0)
				result += "0";
		}
	}
	else if (CheckTypeofQFloat(integral) == QFloat::Zero) // only fractional part
	{
		int exponent = -1;
		result = print_from_fractional_part(fractional, exponent);
		if (-exponent + result.length() > 6)
		{
			if (result.length() > 1)
				result.insert(1, ".");
			result += "e" + std::to_string(exponent);
		}
		else
		{
			result = "0." + result;
			while (exponent++ < -1)
				result.insert(2, "0");
		}
	}
	else
	{
		int exponent = 0;
		result = print_from_integral_part(integral, exponent);
		while (exponent-- > 0)
			result += "0";

		int point_locate = result.length();
		result += "." + print_from_fractional_part(fractional, exponent, point_locate);
		while (exponent++ < -1)
			result.insert(point_locate + 1, "0");

		if (point_locate > 6)
		{
			result.erase(point_locate, 1);
			result.insert(1, ".");
			result += "e" + std::to_string(point_locate - 1);
		}
	}
	if (this->getBit(127) == 1)
		result.insert(0, "-");
	return result;
}

std::string QFloat::toBinString() const
{
	std::string out;
	for (int i = 127; i >= 0; i--)
		out += (this->getBit(i) + '0');
	return out;
}

void round_overflow_digits(std::string& str)
{
	int i = 0;
	while (str[i] == '0' || str[i] == '.')
		i++;
	i += 33;
	for (; i < int(str.length()); i++)
	{
		if (str[i] != '.')
			str[i] = '0';
	}
}

QFloat calculate_from_integral_part(const std::string& src)	// exponent if pass must > 0
{
	QFloat result;
	for (int i = 0; i < int(src.length()); i++)
	{
		result = result * _10 + _BCD[src[i] - '0'];
	}
	return result;
}

QFloat calculate_from_fraction_part(const std::string& src)	// exponent if pass must < 0
{
	QFloat result;
	for (int i = src.length() - 1; i >= 0; i--)
	{
		result = (result + _BCD[src[i] - '0']) / _10;
	}
	return result;
}


void QFloat::fromDecString(std::string src)
{
	memset(bytes, 0, 16);

	// PREPROCESSING
	// B1: Sign
	bool sign = 0;
	if (src[0] == '-')
	{
		sign = 1;
		src.erase(0, 1);
	}
	// B2: Find exponent
	int exp_locate = src.find('e');
	if (exp_locate == std::string::npos)
	{
		exp_locate = src.find('E');
		if (exp_locate == std::string::npos)
			exp_locate = -1;
	}
	int exponent = 0;
	if (exp_locate != -1)	// extract exponent if exist
	{
		exponent = std::stoi(src.substr(exp_locate + 1));
		if (exponent > 4932)
		{
			if (this->getBit(127) == 0)
				*this = _pINF;
			else
				*this = _nINF;
			return;
		}
		if (exponent < -4966)
		{
			*this = _0;
			return;
		}
		src.erase(exp_locate);
	}
	if (exp_locate != -1 && src == "")
		*this = _1;
	else
	{
		round_overflow_digits(src);

		int point_locate = src.find('.');

		std::string integral_part = src.substr(0, point_locate);
		std::string fractional_part;
		if (point_locate != std::string::npos)
			fractional_part = src.substr(point_locate + 1);

		*this = calculate_from_integral_part(integral_part) + calculate_from_fraction_part(fractional_part);
	}

	if (exponent > 0)
	{
		while (exponent-- > 0)
		{
			*this = *this * _10;

		}
	}
	else if (exponent < 0)
	{
		while (exponent++ < 0)
		{
			*this = *this / _10;
		}
	}

	this->setBit(127, sign);
	return;
}


QFloat operator+(const QFloat& x, const QFloat& y) {

	char checkX = CheckTypeofQFloat(x);
	char checkY = CheckTypeofQFloat(y);

	if (checkX == QFloat::Infinity || checkX == QFloat::Nan || checkY == QFloat::Infinity || checkY == QFloat::Nan) {
		return _NaN;
	}
	else if (checkX == QFloat::Zero) {
		return y;
	}
	else if (checkY == QFloat::Zero) {
		return x;
	}

	QInt first, second, I, Mukq, khong("0"), mot("0b00000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	QFloat F;
	bool WeAreAdding2Denormalize = true;
	bool OverFlowDenormalize = false;

	if (checkX == QFloat::Normal) {
		first.setBit(112, 1);
		WeAreAdding2Denormalize = false;
	}
	else { first.setBit(112, 0); }

	if (checkY == QFloat::Normal) {
		second.setBit(112, 1);
		WeAreAdding2Denormalize = false;
	}
	else { second.setBit(112, 0); }

	for (int i = 0; i <= 111; i++) {

		first.setBit(i, x.getBit(i));
		second.setBit(i, y.getBit(i));

	}

	int compareEXY = CompareExponent(x, y);

	// Neu nhu 2 mu khong bang nhau thi can bang lai 2 so mu
	if (compareEXY != 0) {

		// Neu nhu su chech lech giua 2 so mu qua lon -> 1 so rat lon + 1 so rat be -> kq la  so rat lon
		if (compareEXY > 112) {
			QFloat resulf = x;
			return resulf;
		}
		else if (compareEXY < -112) {
			QFloat resulf = y;
			return resulf;
		}


		if (compareEXY > 0) { /*Neu nhu mu x > mu y thi tang mu y */
			second = second >> compareEXY;
			// Chon mu X de lam mukq
			for (int i = 126; i >= 112; i--)
			{
				Mukq.setBit(i, x.getBit(i));
			}

		}
		else {
			first = first >> abs(compareEXY);  /*Neu nhu mu y > mu x thi tang mu x */
			// Chon mu Y de lam mukq
			for (int i = 126; i >= 112; i--)
			{
				Mukq.setBit(i, y.getBit(i));
			}
		}
	}
	else {
		// Neu nhu mu cua 2 so bang nhau thi lay do lam mu kq
		for (int i = 112; i <= 126; i++) {

			Mukq.setBit(i, x.getBit(i));

		}
	}

	bool dau1 = x.getBit(127);
	bool dau2 = y.getBit(127);

	// Neu nhu 2 so cung dau thi cu cong vao thoai mai
	if (dau1 == dau2) {

		QInt::Congtrongkhoang(first, second, 0, 113);
		I = first;
		F.setBit(127, x.getBit(127));

	}
	else { // Neu 2 so khac dau thi lay so lon tru so be roi lay dau cua so lon

	   // sosanh 2 so xem so nao lon hon
		int sosanh = 0;
		for (int k = 112; k >= 0; k--)
		{
			if (first.getBit(k) < second.getBit(k)) {
				sosanh = -1;
				break;
			}

			if (first.getBit(k) > second.getBit(k)) {
				sosanh = 1;
				break;
			}
		}

		// neu significand first > significand second
		if (sosanh == 1) {

			QInt::Trutrongkhoang(first, second, 0, 113);
			F.setBit(127, x.getBit(127)); // set dau , lay dau la dau cua So dau
			I = first;

		}
		else if (sosanh == -1) {

			QInt::Trutrongkhoang(second, first, 0, 113);
			F.setBit(127, y.getBit(127)); // set dau , lay dau la dau cua so thu 2
			I = second;

		}
		else { // Cung mu cung significand, tuc la ket qua bang 0
			return F;
		}

	}

	// Xem thu la co can phai tang mu len khong, thuong la tang mu len se gap hien tuong truncation
	if (I.getBit(113) == 1 && WeAreAdding2Denormalize == false) {

		// tang so mu cua kq len 1
		QInt::Congtrongkhoang(Mukq, mot, 112, 126);
		I = I >> 1;

	}
	else if (I.getBit(112) == 1 && WeAreAdding2Denormalize == true) {

		QInt::Congtrongkhoang(Mukq, mot, 112, 126);

	}
	else if (I.getBit(112) == 0 && WeAreAdding2Denormalize == false) {

		// Doi voi cac truong hop 2 so tru cho nhau ma khong con o dang chuan
		int sobitcandich2 = 0;
		for (int i = 111; i >= 0; i--) {
			if (I.getBit(i) == 1) {
				sobitcandich2 = 112 - i;
				break;
			}
		}

		// Giam so mu va tang phan significand
		for (int i = 1; i <= sobitcandich2; i++) {

			// Neu mu bang 0 thi luc nay phai chuyen sang dang denorrmal
			if (Mukq == khong) {
				break;
			}
			else {
				I = I << 1;
				QInt::Trutrongkhoang(Mukq, mot, 112, 126);
			}
		}
	}


	// Toi buoc nay roi thi chi can set mu va significand vao lai F
	// Set Significand
	for (int i = 111; i >= 0; i--) {
		F.setBit(i, I.getBit(i));
	}

	// Set Exponent
	for (int i = 126; i >= 112; i--) {
		F.setBit(i, Mukq.getBit(i));
	}

	return F;

}

QFloat operator-(const QFloat& x, const QFloat& y) {
	QFloat second = y;
	if (y.getBit(127) == 0) {
		second.setBit(127, 1);
	}
	else {
		second.setBit(127, 0);
	}
	return x + second;
}

void QFloat::fromBinString(std::string src)
{
	int i = 0;
	for (; i < int(src.length()); i++)
	{
		//set value from input string to our QFloat
		this->setBit(127 - i, src[i] - '0');
	}
	for (i = 127 - i; i >= 0; i--)
	{
		//if the string has run out, just set bit to zero
		this->setBit(i, 0);
	}
}

QFloat::QFloat()
{
	memset(bytes, 0, 16);
}

QFloat::QFloat(const QFloat& src): QFloat()
{
	for (int i = 0; i < 128; i++)
	{
		this->setBit(i, src.getBit(i));
	}
}

QFloat::QFloat(std::string src)
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


QFloat::QFloat(const char* src) : QFloat(std::string(src))
{ }

QFloat& QFloat::operator=(const QFloat& src)
{
	if (this == &src)
		return *this;

	for (int i = 0; i < 128; i++)
	{
		this->setBit(i, src.getBit(i));
	}
	return *this;
}

QFloat& QFloat::operator=(std::string srcStr)
{
	return *this = QFloat(srcStr);
}

QFloat& QFloat::operator=(const char* srcStr)
{
	return *this = QFloat(std::string(srcStr));
}

std::istream& operator>>(std::istream& istr, QFloat& qf)
{
	// TODO: insert return statement here
	return istr;
}

std::ostream& operator<<(std::ostream& ostr, const QFloat& qf)
{
	ostr << qf.toDecString();
	return ostr;
}


QFloat operator*(const QFloat& x, const QFloat& y)
{
	QFloat opr1 = x;
	QFloat opr2 = y;
	QFloat res = "0b0";
	int und = QFloat::NUMBITS - 16;
	opr1.setBit(127, opr1.getBit(127) != opr2.getBit(127));
	opr2.setBit(127, opr1.getBit(127));

	//if x==0
	bool is_ = true;
	for (int i = 0; i < QFloat::NUMBITS - 1; i++)
	{
		if (opr1.getBit(i) == 1) { is_ = false; break; }
	}
	if (is_) { return res; }
	//if y==0
	is_ = true;
	for (int i = 0; i < QFloat::NUMBITS - 1; i++)
	{
		if (opr2.getBit(i) == 1) { is_ = false; break; }
	}
	if (is_) { return res; }
	//if(x==infinity(1) or x==Nan(2))
	char xis_ = 1;
	for (int i = und; i <= 126; i++)
	{
		if (opr1.getBit(i) != 1) { xis_ = 0; break; }
	}
	if (xis_ == 1) {
		for (int i = und - 1; i >= 0; i--)
		{
			if (opr1.getBit(i) == 1) { xis_ = 2; break; }
		}
	}
	//y=infinity(1) or y=nan(2)
	char yis_ = 1;
	for (int i = und; i <= 126; i++)
	{
		if (opr2.getBit(i) != 1) { yis_ = 0; break; }
	}
	if (yis_ == 1)
	{
		for (int i = und - 1; i >= 0; i--)
		{
			if (opr2.getBit(i) == 1) { yis_ = 2; break; }
		}
	}
	if (xis_ == 2) { return opr1; }
	else if (yis_ == 2) { return opr2; }
	else if (xis_ == 1) { return opr1; }
	else if (yis_ == 1) { return opr2; }

	res = opr1;
	/////
	int bias = 16383;
	int exp1 = 0;
	int exp2 = 0;
	char opr1s[113] = { 0 };
	char opr2s[113] = { 0 };
	char oprres[226] = { 0 };


	//get raw exp from two number
	for (int i = und; i < QFloat::NUMBITS - 1; i++)
	{
		exp1 |= (1 << (i - und)) * opr1.getBit(i);
		exp2 |= (1 << (i - und)) * opr2.getBit(i);
	}
	//////////////
	///opr1s operand1 significant
	int denormalize_gap = 0;//exp gap with normalize form
	if (exp1 == 0)//denormalize
	{
		denormalize_gap = 1;
		//exp1--;
		while (opr1.getBit(112 - denormalize_gap) == 0)
		{
			denormalize_gap++;
			exp1--;
		}
		for (int i = 112; i >= denormalize_gap; i--)
		{
			opr1s[i] = opr1.getBit(i - denormalize_gap);
		}
	}
	else
	{
		for (int i = 111; i >= 0; i--)
		{
			opr1s[i] = opr1.getBit(i);
		}
		opr1s[112] = 1;
	}

	////opr2s operand2 significant 
	denormalize_gap = 0;//exp gap with normalize form
	if (exp2 == 0)//denormalize
	{
		denormalize_gap = 1;
		//exp1--;

		while (opr2.getBit(112 - denormalize_gap) == 0)
		{
			denormalize_gap++;
			exp2--;
		}
		for (int i = 112; i >= denormalize_gap; i--)
		{
			opr2s[i] = opr2.getBit(i - denormalize_gap);
		}
	}
	else
	{
		for (int i = 111; i >= 0; i--)
		{
			opr2s[i] = opr2.getBit(i);
		}
		opr2s[112] = 1;
	}
	//////////

	exp1 = exp1 + exp2 - bias;

	/////
	//multiply significant
	for (int i = 0; i <= 112; i++)//i-start index in res to plus opr1
	{
		if (opr2s[i] != 0)
		{	//plus
			char remember = 0;

			int j = 0;
			for (; j <= 112; j++)
			{
				char temp = oprres[i + j] + opr1s[j] + remember;
				oprres[i + j] = temp % 2;
				remember = temp / 2;
			}

			oprres[i + j] = remember;
		}
	}


	//
	int afterdecimal = 223;
	if (oprres[225] == 1)
	{
		afterdecimal = 224;
		exp1 += 1;
	}
	//round
	if (oprres[afterdecimal - 112] == 1)
	{
		if (oprres[afterdecimal - 111] == 1 || oprres[afterdecimal - 113] == 1)
		{
			//add one smallest bit
			char remember = 1;

			int j = 0;
			for (; j <= 111; j++)
			{
				char temp = oprres[afterdecimal - 111 + j] + remember;
				oprres[afterdecimal - 111 + j] = temp % 2;
				remember = temp / 2;
			}
			if (remember == 1)
			{
				afterdecimal++;
				oprres[afterdecimal] = 0;
				exp1 += 1;
			}
		}
	}

	///set last bit to common pattern if there is a common pattern
	int lastdigit = afterdecimal - 111;
	int common_pattern_last_for = 0;
	int i = afterdecimal;
	for (; i > lastdigit; i--)
	{
		if (oprres[i + 1] != oprres[i]) { common_pattern_last_for = 0; }
		else { common_pattern_last_for += 1; }
	}
	if (common_pattern_last_for >= 10) { oprres[i] = oprres[i + 1]; }
	///////////
	///overflow
	if (exp1 > QFloat::BIAS * 2)//overflow, raw exp >bias*2 mean exponent of opr1>bias
	{
		for (int i = und; i < QFloat::NUMBITS - 1; i++)
		{
			res.setBit(i, 1);
		}
		for (int i = 0; i < und; i++)
		{
			res.setBit(i, 0);
		}
		return res;
	}
	//underflow
	if (exp1 < -(QFloat::NUMBITS - 16))//underflow
	{
		for (int i = 0; i < QFloat::NUMBITS; i++)
		{
			res.setBit(i, 0);
		}
		return res;
	}
	/////
	if (exp1 < 1)
	{
		for (int i = und; i < 127; i++)
		{
			res.setBit(i, 0);
		}
		exp1++;
		afterdecimal++;

		int i = 111;
		while (exp1 < 1)
		{
			res.setBit(i, 0);
			i--;
			exp1++;
		}

		while (i >= 0)
		{
			res.setBit(i, oprres[afterdecimal]);
			i--;
			afterdecimal--;
		}
	}
	else
	{	//exp
		for (int i = 0; i < 15; i++)
		{
			res.setBit(und + i, (exp1 >> i) & 1);
		}
		for (int i = 111; i >= 0; i--)
		{
			res.setBit(i, oprres[afterdecimal]);
			afterdecimal--;
		}
	}

	/////
	return res;
}
QFloat operator/(const QFloat& x, const QFloat& y)
{
	QFloat opr1 = x;
	QFloat opr2 = y;
	QFloat res = "0b0";
	int und = QFloat::NUMBITS - 16;
	opr1.setBit(127, opr1.getBit(127) != opr2.getBit(127));
	opr2.setBit(127, opr1.getBit(127));

	//if x==0
	bool xis_0 = true;
	for (int i = 0; i < QFloat::NUMBITS - 1; i++)
	{
		if (opr1.getBit(i) == 1) { xis_0 = false; }
	}

	bool yis_0 = true;
	for (int i = 0; i < QFloat::NUMBITS - 1; i++)
	{
		if (opr2.getBit(i) == 1) { yis_0 = false; }
	}

	if (yis_0) {//return Nan
		for (int i = und; i < QFloat::NUMBITS - 1; i++)
		{
			res.setBit(i, 1);
		}
		for (int i = und - 3; i < und; i++)
		{
			res.setBit(i, 1);
		}
		return res;
	}

	//if(x==infinity or x==Nan)
	char xis_ = 1;
	for (int i = und; i <= 126; i++)
	{
		if (opr1.getBit(i) != 1) { xis_ = 0; break; }
	}
	if (xis_ == 1) {
		for (int i = und - 1; i >= 0; i--)
		{
			if (opr1.getBit(i) == 1) { xis_ = 2; break; }
		}
	}
	//y=infinity or y=nan
	char yis_ = 1;
	for (int i = und; i <= 126; i++)
	{
		if (opr2.getBit(i) != 1) { yis_ = 0; break; }
	}
	if (yis_ == 1)
	{
		for (int i = und - 1; i >= 0; i--)
		{
			if (opr2.getBit(i) == 1) { yis_ = 2; break; }
		}
	}

	if (xis_ == 2) { return opr1; }
	else if (yis_ == 2) { return opr2; }
	if (xis_0) { return res; }//0
	else if (xis_ == 1) { return opr1; }
	else if (yis_ == 1) { return res; }

	res = opr1;
	/////
	//
	int bias = 16383;
	int exp1 = 0;
	int exp2 = 0;
	char oprres[115] = { 0 };
	char opr2s[113] = { 0 };
	char opr1s[228] = { 0 };

	//get raw exp from two number
	for (int i = und; i < QFloat::NUMBITS - 1; i++)
	{
		exp1 |= (1 << (i - und)) * opr1.getBit(i);
		exp2 |= (1 << (i - und)) * opr2.getBit(i);
	}
	//////////////
	///opr1s operand1 significant
	int denormalize_gap = 0;//exp gap with normalize form
	if (exp1 == 0)//denormalize
	{
		denormalize_gap = 1;
		while (opr1.getBit(112 - denormalize_gap) == 0)
		{
			denormalize_gap++;
			exp1--;
		}
		for (int i = 112; i >= denormalize_gap; i--)
		{
			opr1s[i] = opr1.getBit(i - denormalize_gap);
		}
	}
	else
	{
		for (int i = 111; i >= 0; i--)
		{
			opr1s[i] = opr1.getBit(i);
		}
		opr1s[112] = 1;
	}
	for (int i = 226; i >= 114; i--)
	{
		opr1s[i] = opr1s[i - 114];
	}

	////opr2s operand2 significant 
	denormalize_gap = 0;//exp gap with normalize form
	if (exp2 == 0)//denormalize
	{
		denormalize_gap = 1;
		while (opr2.getBit(112 - denormalize_gap) == 0)
		{
			denormalize_gap++;
			exp2--;
		}
		for (int i = 112; i >= denormalize_gap; i--)
		{
			opr2s[i] = opr2.getBit(i - denormalize_gap);
		}
	}
	else
	{
		for (int i = 111; i >= 0; i--)
		{
			opr2s[i] = opr2.getBit(i);
		}
		opr2s[112] = 1;
	}
	//////////

	exp1 = exp1 - exp2 + bias;

	//////////////////////////////////
	//divide significant
	int opr1_vs_opr2 = 0;
	for (int i = 114; i >= 0; i--)
	{
		//compare
		opr1_vs_opr2 = 1;
		if (opr1s[i + 113] == 0)
		{
			for (int kt = 0; kt <= 112; kt++)
			{
				opr1_vs_opr2 = opr1s[i + 112 - kt] - opr2s[112 - kt];

				if (opr1_vs_opr2 != 0) { break; }
			}
		}

		//if after substract we dont get a negative
		if (opr1_vs_opr2 >= 0)
		{
			bool remember = 0;
			//subtract
			for (int j = 0; j <= 112; j++)
			{
				int temp = opr1s[i + j] - (remember + opr2s[j]);
				opr1s[i + j] = temp + (temp < 0) * 2;
				remember = temp < 0;
			}

			oprres[i] = 1;
		}
		else//if after substract we get a negative
		{
			oprres[i] = 0;
		}

	}


	//round
	if (oprres[1] == 1)
	{
		if (oprres[2] == 1 || oprres[0] == 1)
		{
			//add one smallest bit
			char remember = 1;

			int j = 2;
			for (; j <= 114; j++)
			{
				char temp = oprres[j] + remember;
				oprres[j] = temp % 2;
				remember = temp / 2;
			}
			//last remember will be 0, 'cause of math twist
		}
	}

	///set last bit to common pattern if there is a common pattern
	int lastdigit = 2;
	int common_pattern_last_for = 0;
	int i = 113;
	for (; i > lastdigit; i--)
	{
		if (oprres[i + 1] != oprres[i]) { common_pattern_last_for = 0; }
		else { common_pattern_last_for += 1; }
	}
	if (common_pattern_last_for >= 10) { oprres[i] = oprres[i + 1]; }
	//
	int afterdecimal = 113;
	if (oprres[114] == 0)
	{
		afterdecimal = 112;
		exp1 -= 1;
	}

	///////////
	///overflow
	if (exp1 > QFloat::BIAS * 2 + 1)//overflow, raw exp >bias*2+1 mean exponent of opr1>bias
	{
		//infinity
		for (int i = und; i < QFloat::NUMBITS - 1; i++)
		{
			res.setBit(i, 1);
		}
		for (int i = 0; i < und; i++)
		{
			res.setBit(i, 0);
		}
		return res;
	}
	//underflow
	if (exp1 < -(QFloat::NUMBITS - 16))//underflow
	{	//0
		for (int i = 0; i < QFloat::NUMBITS; i++)
		{
			res.setBit(i, 0);
		}
		return res;
	}
	////////////////////////////
	if (exp1 < 1)
	{
		for (int i = und; i < 127; i++)
		{
			res.setBit(i, 0);
		}
		exp1++;
		afterdecimal++;

		int i = 111;
		while (exp1 < 1)
		{
			res.setBit(i, 0);
			i--;
			exp1++;
		}

		while (i >= 0)
		{
			res.setBit(i, oprres[afterdecimal]);
			i--;
			afterdecimal--;
		}
	}
	else
	{	//exp
		for (int i = 0; i < 15; i++)
		{
			res.setBit(und + i, (exp1 >> i) & 1);
		}
		for (int i = 111; i >= 0; i--)
		{
			res.setBit(i, oprres[afterdecimal]);
			afterdecimal--;
		}
	}
	/////

	return res;
}
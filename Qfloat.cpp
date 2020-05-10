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

// ham bo tro 

// so sanh 2 mu cua 2 so Qfloat, tra ve do chech lech 
// Neu x > y tra ve so duong
// Neu x < y tra ve so am
int CompareExponent(const Qfloat& x, const Qfloat& y) {

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

void cut_off_meaningful_digits(std::string& str)
{
	if (str.length() > 33)
		str.erase(33);
}

Qfloat calculate_from_integral_part(const std::string& src, int exponent = 0)	// exponent if pass must > 0
{
	Qfloat result;
	for (int i = 0; i < int(src.length()); i++)
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

Qfloat calculate_from_fraction_part(const std::string& src, int exponent = 0)	// // exponent if pass must < 0
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

int move_floating_point_based_on_exponent(std::string& str, int& exponent) // exponent >= 0
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


bool Kiemtramubang0(const Qfloat& x) {
	for (int i = 126; i >= 112; i--) {
		if (x.getBit(i) == 1) { return false; }
	}
	return true;
}

bool Kiemtrabang0(const Qfloat& x) {
	for (int i = 126; i >= 0; i--) {
		if (x.getBit(i) == 1) { return false; }
	}
	return true;
}

bool IsInfinityOrNaN(const Qfloat& x) {
	for (int i = 126; i >= 112; i--) {
		if (x.getBit(i) != 1) { return false; }
	}
	return true;
}

Qfloat operator+(const Qfloat& x, const Qfloat& y) {

	if (IsInfinityOrNaN(x) == true || IsInfinityOrNaN(y) == true) {
		Qfloat trave;
		trave.fromBinString("011111111111111111111");
		return trave;
	}

	if (Kiemtrabang0(x)) {
		return y;
	}
	else if (Kiemtrabang0(y)) {
		return x;
	}

	QInt first, second, I, Mukq, khong("0"), mot("0b00000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	Qfloat F;
	bool WeAreAdding2Denormalize = true;
	bool OverFlowDenormalize = false;

	if (Kiemtramubang0(x) == false) {

		first.setBit(112, 1);
		WeAreAdding2Denormalize = false;

	}
	else {

		first.setBit(112, 0);

	}

	if (Kiemtramubang0(y) == false) {

		second.setBit(112, 1);
		WeAreAdding2Denormalize = false;

	}
	else {

		second.setBit(112, 0);

	}

	for (int i = 0; i <= 111; i++) {

		first.setBit(i, x.getBit(i));
		second.setBit(i, y.getBit(i));

	}

	int compareEXY = CompareExponent(x, y);

	// Neu nhu 2 mu khong bang nhau thi can bang lai 2 so mu
	if (compareEXY != 0) {

		// Neu nhu su chech lech giua 2 so mu qua lon -> 1 so rat lon + 1 so rat be -> kq la  so rat lon
		if (compareEXY > 112) {
			Qfloat resulf = x;
			return resulf;
		}
		else if (compareEXY < -112) {
			Qfloat resulf = y;
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


	////// FOR DEBUG
	//std::cout << "7654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	//for (int i = 127; i >= 0; i--)
	//{
	//	std::cout << I.getBit(i);
	//}
	//std::cout << std::endl;
	//for (int i = 127; i >= 0; i--)
	//{
	//	std::cout << Mukq.getBit(i);
	//}

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

Qfloat operator-(const Qfloat& x, const Qfloat& y) {
	Qfloat second = y;
	if (y.getBit(127) == 0) {
		second.setBit(127, 1);
	}
	else {
		second.setBit(127, 0);
	}
	return x + second;
}

void Qfloat::fromBinString(std::string src)
{
	int i = 0;
	for (; i < int(src.length()); i++)
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

Qfloat::Qfloat(const Qfloat& src)
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


Qfloat::Qfloat(const char* src)
{
	*this = Qfloat(std::string(src));
}

Qfloat& Qfloat::operator=(const Qfloat& src)
{
	for (int i = 0; i < 128; i++)
	{
		this->setBit(i, src.getBit(i));
	}
	return *this;
}

Qfloat& Qfloat::operator=(std::string srcStr)
{
	*this = Qfloat(srcStr);

	return *this;
}

Qfloat& Qfloat::operator=(const char* srcStr)
{
	*this = Qfloat(std::string(srcStr));
	return *this;
}

std::ostream& operator<<(std::ostream& ostr, const Qfloat& qf)
{
	ostr << qf.toDecString();
	return ostr;
}

Qfloat operator*(const Qfloat& x, const Qfloat& y)
{
	Qfloat opr1 = x;
	Qfloat opr2 = y;
	Qfloat res = "0b0";

	//if x==0
	bool is_0 = true;
	for (int i = 0; i < Qfloat::NUMBITS - 1; i++)
	{
		if (opr1.getBit(i) == 1) { is_0 = false; }
	}

	if (is_0) { return res; }

	is_0 = true;
	for (int i = 0; i < Qfloat::NUMBITS - 1; i++)
	{
		if (opr2.getBit(i) == 1) { is_0 = false; }
	}

	if (is_0) { return res; }

	res = opr1;
	res.setBit(Qfloat::NUMBITS - 1, opr1.getBit(Qfloat::NUMBITS - 1) != opr2.getBit(Qfloat::NUMBITS - 1));
	/////
	//
	int bias = 16383;
	int und = Qfloat::NUMBITS - 16;
	int exp1 = 0;
	int exp2 = 0;
	char opr1s[113] = { 0 };
	char opr2s[113] = { 0 };
	char oprres[226] = { 0 };


	//get raw exp from two number
	for (int i = und; i < Qfloat::NUMBITS - 1; i++)
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

	///////////
	///overflow
	if (exp1 > Qfloat::BIAS * 2 + 1)//overflow, raw exp >bias*2+1 mean exponent of opr1>bias
	{
		res.setBit(Qfloat::NUMBITS - 1, opr1.getBit(Qfloat::NUMBITS - 1) != opr2.getBit(Qfloat::NUMBITS - 1));//sign

		for (int i = und; i < Qfloat::NUMBITS - 1; i++)
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
	if (exp1 < -(Qfloat::NUMBITS - 16))//underflow
	{
		for (int i = 0; i < Qfloat::NUMBITS; i++)
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

	//set sign
	res.setBit(Qfloat::NUMBITS - 1, opr1.getBit(Qfloat::NUMBITS - 1) != opr2.getBit(Qfloat::NUMBITS - 1));

	return res;
}
Qfloat operator/(const Qfloat& x, const Qfloat& y)
{
	Qfloat opr1 = x;
	Qfloat opr2 = y;
	Qfloat res = "0b0";
	int und = Qfloat::NUMBITS - 16;

	//if x==0
	bool is_0 = true;
	for (int i = 0; i < Qfloat::NUMBITS - 1; i++)
	{
		if (opr1.getBit(i) == 1) { is_0 = false; }
	}

	if (is_0) { return res; }//0

	is_0 = true;
	for (int i = 0; i < Qfloat::NUMBITS - 1; i++)
	{
		if (opr2.getBit(i) == 1) { is_0 = false; }
	}

	if (is_0) {
		for (int i = und; i < Qfloat::NUMBITS - 1; i++)
		{
			res.setBit(i, 1);
		}
		for (int i = und - 3; i < und; i++)
		{
			res.setBit(i, 1);
		}
		return res;
	}

	res = opr1;
	res.setBit(Qfloat::NUMBITS - 1, opr1.getBit(Qfloat::NUMBITS - 1) != opr2.getBit(Qfloat::NUMBITS - 1));
	/////
	//
	int bias = 16383;
	int exp1 = 0;
	int exp2 = 0;
	char oprres[113] = { 0 };
	char opr2s[113] = { 0 };
	char opr1s[227] = { 0 };

	//get raw exp from two number
	for (int i = und; i < Qfloat::NUMBITS - 1; i++)
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
	for (int i = 225; i >= 113; i--)
	{
		opr1s[i] = opr1s[i - 113];
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
	for (int i = 113; i >= 1; i--)
	{
		opr1_vs_opr2 = 1;
		if (opr1s[i + 113] == 0)
		{
			for (int kt = 0; kt < 113; kt++)
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
			for (int j = 0; j <= 113; j++)
			{
				int temp = opr1s[i + j] - (remember + opr2s[j]);
				opr1s[i + j] = temp + (temp < 0) * 2;
				remember = temp < 0;
			}

			oprres[i - 1] = 1;
		}
		else//if after substract we get a negative
		{
			oprres[i - 1] = 0;
		}

	}

	//
	int afterdecimal = 111;
	if (oprres[112] == 0)
	{
		afterdecimal = 110;
		exp1 -= 1;
	}

	///////////
	///overflow
	if (exp1 > Qfloat::BIAS * 2 + 1)//overflow, raw exp >bias*2+1 mean exponent of opr1>bias
	{
		res.setBit(Qfloat::NUMBITS - 1, opr1.getBit(Qfloat::NUMBITS - 1) != opr2.getBit(Qfloat::NUMBITS - 1));//sign
		//infinity
		for (int i = und; i < Qfloat::NUMBITS - 1; i++)
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
	if (exp1 < -(Qfloat::NUMBITS - 16))//underflow
	{	//0
		for (int i = 0; i < Qfloat::NUMBITS; i++)
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

	//set sign
	res.setBit(Qfloat::NUMBITS - 1, opr1.getBit(Qfloat::NUMBITS - 1) != opr2.getBit(Qfloat::NUMBITS - 1));

	return res;
}


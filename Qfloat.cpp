#include "Qfloat.h"
#include "QInt.h"

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
Qfloat _BCD[11] = { _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10 };

Qfloat _pINF = "0b0111111111111111";

Qfloat _nINF = "0b1111111111111111";

Qfloat _NaN = "0b0111111111111111111";

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

int Qfloat::get_exponent() const // of 2, not 10
{
	std::string exp_str;
	for (int i = 126; i > 111; i--)
		exp_str += (this->getBit(i) + '0');
	return std::stoi(exp_str, 0, 2) - 16383;
	/*int x = 0;
	for (int i = 126, int j = 0; i > 111; i--, j--)
	{
		bool b = this->getBit(i);
		if (b == 1)
			x = x & (1 >> j);
	}
	return x - 16383;*/
}



void Qfloat::modf(Qfloat &integral, Qfloat &fractional) const
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
		/*bool sign = this->getBit(127);
		integral.setBit(127, sign);
		fractional.setBit(127, sign);*/
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
		// ko có trường hợp denormalized form chi ở đây hết :>
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
			// Significant of fraction part is from that first bit 1 to end in the original
			for (; o >= 0; o--, f--)
			{
				fractional.setBit(f, this->getBit(o));
			}
		}
	}
}

//bool operator==(const Qfloat &a, const Qfloat &b)	// dùng để so sánh với số 0 thôi => vậy thôi thà chi đặt lại tên hàm luôn?
//{
//	for (int i = 0; i < 127; i++)	// operator == don't care about sign bit  
//		if (a.getBit(i) != b.getBit(i))
//			return false;
//	return true;
//}

bool isZero(const Qfloat &a)
{
	for (int i = 0; i < 127; i++)	// don't care about sign bit  
		if (a.getBit(i) != 0)
			return false;
	return true;
}

bool almost_equal(const Qfloat &a, const Qfloat &b)	// dùng để nhận diện các chữ số trong bảng BCD
{
	for (int i = 5; i < 127; i++)	//  don't care about sign bit , vì trong bảng cũng có số 0. Chạy từ 5 là bỏ qua 5 bit đầu, kiểu như epsilon
		if (a.getBit(i) != b.getBit(i))
			return false;
	return true;
}

bool strict_equal(const Qfloat &a, const Qfloat &b)	// chỉ dùng để nhận diện +inf và -inf
{
	for (int i = 0; i < 128; i++)	// this function care about sign bit  
		if (a.getBit(i) != b.getBit(i))
			return false;
	return true;
}

bool isNaN(const Qfloat &a)
{
	for (int i = 127; i >= 111; i--)
		if (a.getBit(i) != 1)
			return false;
	return true;
}

//bool operator!=(const Qfloat &a, const Qfloat &b)
//{
//	return !(a == b);
//}

char Qfloat::toChar() const
{
	for (int i = 0; i < 10; i++)
		if (almost_equal(*this, _BCD[i]))
			return i + '0';
	return '?';
}

std::string print_from_integral_part(Qfloat &src, int &exponent)
{
	// exponent = 0;
	Qfloat digit_extracted;	// in form of 0.x
	std::string result;
	// src is alway integral round
	// recognize zero chain from point, go left to meet the first meaningful digit
	while (true)
	{
		Qfloat integral_with_1_digit_after_point = src / _10;
		integral_with_1_digit_after_point.modf(src, digit_extracted);
		if (isZero(digit_extracted))
			exponent++;
		else break;
	}
	while (true) {
		digit_extracted = digit_extracted * _10; // 0.3 -> 3
		result = digit_extracted.toChar() + result;
		if (isZero(src))
			break;
		Qfloat integral_with_1_digit_after_point = src / _10;
		integral_with_1_digit_after_point.modf(src, digit_extracted);
	}
	return result;
}

std::string print_from_fractional_part(Qfloat &src, int &exponent)
{
	// exponent = -1;
	Qfloat digit_extracted;	// in form of x
	std::string result;
	// src is alway a fraction less than 1 : 0.xxx...
	// recognize zero chain from point, go right to meet the first meaningful digit
	while (true)
	{
		Qfloat fractional_with_1_digit_before_point = src * _10;
		fractional_with_1_digit_before_point.modf(digit_extracted, src);
		if (isZero(digit_extracted))
			exponent--;
		else break;
	}
	while (true) {
		result = result + digit_extracted.toChar();
		if (isZero(src))
			break;
		Qfloat fractional_with_1_digit_before_point = src * _10;
		fractional_with_1_digit_before_point.modf(digit_extracted, src);
	}
	return result;
}

std::string Qfloat::toDecString() const
{
	// HARDCORE
	if (isZero(*this))
		return "0";
	if (strict_equal(*this, _pINF))
		return "+inf";
	if (strict_equal(*this, _nINF))
		return "-inf";
	if (isNaN(*this))
		return "NaN";

	Qfloat integral, fractional;
	this->modf(integral, fractional);
	std::string result;
	// chia 2 th: e >= 0 và e < 0, dựa vào integral >= 1 hay ko
	// integral = 1 thì e=0, > 1 thì e > 0, =0 thì e <0
	if (isZero(fractional)) // only integral part
	{
		int exponent = 0;
		result = print_from_integral_part(integral, exponent);
		if (exponent + result.length() > 6)
		{
			result.insert(1, ".");
			result += "e" + std::to_string(exponent + result.length() - 1);
		}
		else
		{
			while (exponent-- > 0)
				result += "0";
		}
	}
	else if (isZero(integral)) // only fractional part
	{
		int exponent = -1;
		result = print_from_fractional_part(fractional, exponent);
		if (exponent + result.length() > 3)
		{
			result.insert(1, ".");
			result += "e" + std::to_string(exponent);
		}
		else
		{
			while (exponent++ < 0)
				result = "0" + result;
		}
	}
	else
	{
		int exponent = 0;
		result = print_from_integral_part(integral, exponent);
		while (exponent-- > 0)
			result += "0";
		int point_locate = result.length();
		result += "." + print_from_fractional_part(fractional, exponent);
		while (exponent++ < 0)
			result.insert(point_locate, "0");

		if (result.length() > 8)
		{
			result.erase(point_locate, 1);
			result.insert(1, ".");
			result += "e" + std::to_string(point_locate);
		}
	}
	if (this->getBit(127) == 1)
		result.insert(0, "-");
	return result;
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
	// phải có bước kiểm tra tràn chưa để stop tại đó, ko cho chạy nữa
	Qfloat result;
	for (int i = 0; i < int(src.length()); i++)
	{
		if (src[i] != '0')
			result = result * _10 + _BCD[src[i] - '0'];
		else
			result = result * _10;
	}
	while (exponent-- > 0)
		result = result * _10;
	return result;
}

Qfloat calculate_from_fraction_part(const std::string &src, int exponent = 0)	// exponent if pass must < 0
{
	Qfloat result;
	for (int i = src.length() - 1; i >= 0; i--)
	{
		if (src[i] != '0')
			result = (result + _BCD[src[i] - '0']) / _10;
		else
			result = result / _10;
	}
	while (exponent++ < 0)
		result = result / _10;
	return result;
}

int locate_fix_point_based_on_exponent(std::string &str, int &exponent) // exponent >= 0
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
	memset(bytes, 0, 16);
	// xử lý tràn trong này ntn?
	// các th tràn ở đây là gì? 
	// 1. mũ lớn hơn 16383*log(2,10) ~ 4931.7: thì overflow, nhưng nhỏ hơn chưa chắc đã ko tràn
	// 2. mũ nhỏ hơn -16495*log(2,10) ~ -4965.4: thì underflow
	// 3. meaningful digits nhiều hơn 33, thì làm tròn, cắt bớt
	// có vấn đề gì với denormalized number ở đây ko? chắc là ko, vì từ góc độ số hệ thập phân, làm sao biết được?


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
		if (exponent > 4931)
		{
			if (this->getBit(127) == 0)
				*this = _pINF;
			else
				*this = _nINF;
			return;
		}
		if (exponent < -4965)
		{
			*this = _0;
			return;
		}
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
			int point_locate = locate_fix_point_based_on_exponent(src, exponent);
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
		return _NaN;
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


Qfloat::Qfloat(const char* src) : Qfloat(std::string(src))
{ }

Qfloat& Qfloat::operator=(const Qfloat& src)
{
	if (this == &src)
		return *this;

	for (int i = 0; i < 128; i++)
	{
		this->setBit(i, src.getBit(i));
	}
	return *this;
}

Qfloat& Qfloat::operator=(std::string srcStr)
{
	return *this = Qfloat(srcStr);
}

Qfloat& Qfloat::operator=(const char* srcStr)
{
	return *this = Qfloat(std::string(srcStr));
}

std::istream & operator>>(std::istream & istr, Qfloat & qf)
{
	// TODO: insert return statement here
	return istr;
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
		//infinity

		/*res.setBit(Qfloat::NUMBITS - 1, opr1.getBit(Qfloat::NUMBITS - 1) != opr2.getBit(Qfloat::NUMBITS - 1));//sign
		for (int i = und; i < Qfloat::NUMBITS - 1; i++)
		{
			res.setBit(i, 1);
		}
		for (int i = 0; i < und; i++)
		{
			res.setBit(i, 0);
		}
		return res;*/
		res = opr1.getBit(Qfloat::NUMBITS - 1) != opr2.getBit(Qfloat::NUMBITS - 1) ? _nINF : _pINF;
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


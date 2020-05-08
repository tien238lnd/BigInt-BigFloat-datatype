#pragma once
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
int CompareExponent(const Qfloat& x,const Qfloat& y){
	for(int i = 126; i >= 112 ; i--){
		if(x.getBit(i) < y.getBit(i)){
			return -1;
			// cho nay nen break ra
		}else if(x.getBit(i) > y.getBit(i)){
			return 1;
			// cho nay nen break ra
		}
	}


	// sau khi break thi tiep tuc cho chay tu dau tro ve cuoi
	return 0;
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


bool Kiemtramubang0(const Qfloat& x){
	for(int i = 126; i>=112;i-- ){
		if(x.getBit(i)==1){return false;}
	}
	return true;
}

bool Kiemtrabang0(const Qfloat& x){
	for(int i = 126; i>=0;i-- ){
		if(x.getBit(i)==1){return false;}
	}
	return true;
}

Qfloat operator+( const Qfloat& x,  const Qfloat& y)  {

	if(Kiemtrabang0(x)){
		return y;
	}else if(Kiemtrabang0(y)){
		return x;
	}

	QInt first;
	QInt second;
	QInt I;
	QInt Mukq;
	Qfloat F;
	QInt mot("1");
	QInt khong("0");
	bool WeAreAdding2Denormalize = true;

	if(Kiemtramubang0(x)==false){
		
		first.setBit(112,1);
		WeAreAdding2Denormalize = false;

	}else{

		first.setBit(112,0);

	}
	
	if(Kiemtramubang0(y) == false){

		second.setBit(112,1);
		WeAreAdding2Denormalize = false;

	}else{

		second.setBit(112, 0);

	}

	for(int i = 0; i<= 111; i++){

		first.setBit(i,x.getBit(i));
		second.setBit(i,y.getBit(i));

	}

	int compareEXY = CompareExponent(x,y);

	// Neu nhu 2 mu khong bang nhau thi can bang lai 2 so mu
	if (compareEXY != 0 ){

		// Neu nhu su chech lech giua 2 so mu qua lon -> 1 so rat lon + 1 so rat be -> kq la  so rat lon
		if(compareEXY > 112 ){
			Qfloat resulf = x;
			return resulf;
		}else if(compareEXY < -112 ){
			Qfloat resulf = y;
			return resulf;
		}


		if(compareEXY > 0){ /*Neu nhu mu x > mu y thi tang mu y */
			second = second >> compareEXY;
			// Chon mu X de lam mukq
			for (int i = 126; i >= 112; i--)
			{
				Mukq.setBit(i, x.getBit(i));
			}

		}else{
			first = first >> abs(compareEXY);  /*Neu nhu mu y > mu x thi tang mu x */
			// Chon mu Y de lam mukq
			for (int i = 126; i >= 112; i--)
			{
				Mukq.setBit(i, y.getBit(i));
			}
		}
	}





	bool dau1 = first.getBit(127);
	bool dau2 = second.getBit(127);

	// Neu nhu 2 so cung dau
	if(dau1 == dau2){

		QInt::Congtrongkhoang(first, second, 0,113);
		I = first;

	}else{ // Neu 2 so khac dau thi lay so lon tru so be roi lay dau cua so lon
	
		// sosanh 2 so xem so nao lon hon
		int sosanh = 0 ;

		for (int k = 112; k >= 0; k--)
		{
			if (first.getBit(k) < second.getBit(k)) {
				sosanh = -1 ;
				break;
			}

			if (first.getBit(k) > second.getBit(k)) {
				sosanh = 1;
				break;
			}
		}

		// neu significand first > significand second
		if(sosanh == 1){

			QInt::Trutrongkhoang(first,second,0,113);
			 F.setBit(127, x.getBit(127)); // set dau , lay dau la dau cua So dau
			 I = first;

		}else if(sosanh == -1){ 

			QInt::Trutrongkhoang(second, first, 0,113);
			 F.setBit(127, y.getBit(127)); // set dau , lay dau la dau cua so thu 2
			 I = second;

		}else{ // Cung mu cung significand, tuc la ket qua bang 0
			return F;
		}
	
	}

	//std::cout << first.DectoBin() << std::endl;
	//std::cout << second.DectoBin() << std::endl;
	//std::cout << F.toBinString() << std::endl;
	//std::cout << Mukq.DectoBin() << std::endl;
	//std::cout << I.DectoBin() << std::endl;
	// Xem thu la co can phai tang mu len khong, thuong la tang mu len se gap hien tuong truncation
	if( I.getBit(113) == 1 && WeAreAdding2Denormalize == false){
		// tang so mu cua 2 so len 1
		Mukq = Mukq + mot; 
		I = I >> 1;	
	}else if( I.getBit(112) == 1 && WeAreAdding2Denormalize == true ){
		Mukq = Mukq + mot;
	}else if(I.getBit(112) == 0 && WeAreAdding2Denormalize == false ){
		
		int sobitcandich2 = 0;
		
		for(int i = 111; i >= 0 ; i--){
			if(I.getBit(i)== 1){
				sobitcandich2 = i;
				break;
			}
		}

		// Giam so mu va tang phan significand
		for(int i = 0; i <= sobitcandich2 ; i++){
			Mukq = Mukq - mot;
			if(Mukq == khong){
				break;
			}else{
				I = I << 1;
			}
		}
	}
	std::cout << I.DectoBin() << std::endl;
	/* 
	if(I.getBit(112) == 0){

		int sobitcandich = 0 ;
		for(int i = 111; i >= 0; i--){
			if(I.getBit(i) == 1){
				sobitcandich = 112 - i;
				break;
			}
		}
		I = I << sobitcandich; 
		// Giam mu di 


	}
	*/

	// toi buoc nay roi thi chi viec set mu voi set significand
	// sau do thi set lai cac bit significand vao trong F roi return
	for(int i = 111; i >= 0; i--){
		F.setBit(i,I.getBit(i));
	}

	// set mu vao trong F
	// Neu nhu no la so Denormalize thi set no la 0 
	for(int i = 126; i >= 112 ; i--){
		if (WeAreAdding2Denormalize) {
			F.setBit(i, 0);
		}
		else {
			F.setBit(i, Mukq.getBit(i));
		}
	}

	return F;

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

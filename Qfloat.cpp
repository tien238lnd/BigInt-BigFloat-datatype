#include "Qfloat.h"
#include "math.h"

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

// Tang Mu cua Qfloat them n lan 
void TangMu(Qfloat& qf, int n){

	for(int j = 1; j<= n; j++){
		
		bool nho = 1;

		for(int i = 112 ; i<=126; i++){
			short int a = qf.getBit(i) ;
			if (a + nho == 2) {
				qf.setBit(i, 0);
				nho = 1;
			}
			else
			{
				qf.setBit(i, a + nho);
				nho = 0;
			}
		}

	}

}

// Copy so mu tu Qfloat X sang Qint Y neu flag =1
// Copy so mu tu Qint Y sang Qfloat X neu flag = 0
void CopyMu(Qfloat& x, QInt& y, bool flag ){
	if(flag == 1){
		for(int i = 126; i >= 112 ; i--){

		}
	}else{

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

Qfloat operator+ ( Qfloat& x,  Qfloat& y)  {

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
			CopyMu(x,Mukq,1);			

		}else{
			first = first >> abs(compareEXY);  /*Neu nhu mu y > mu x thi tang mu x */
			// Chon mu Y de lam mukq
			CopyMu(y,Mukq,1);	
		}
	}


	bool dau1 = first.getBit(127);
	bool dau2 = second.getBit(127);

	// Neu nhu 2 so cung dau
	if(dau1 == dau2){

		QInt::Congtrongkhoang(first, second, 113,0);
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

			QInt::Trutrongkhoang(first,second,113,0);
			 F.setBit(127, x.getBit(127)); // set dau , lay dau la dau cua So dau
			 I = first;

		}else if(sosanh == -1){ 

			QInt::Trutrongkhoang(second, first, 113,0);
			 F.setBit(127, y.getBit(127)); // set dau , lay dau la dau cua so thu 2
			 I = second;

		}else{ // Cung mu cung significand, tuc la ket qua bang 0
			return F;
		}
	
	}

	// Xem thu la co can phai tang mu len khong, thuong la tang mu len se gap hien tuong truncation
	if( I.getBit(113) == 1 && WeAreAdding2Denormalize == false){
		// tang so mu cua 2 so len 1
		Mukq = Mukq + mot; 
		I = I >> 1;	
	}else if( I.getBit(112) == 1 && WeAreAdding2Denormalize == true ){
		Mukq = Mukq + mot;
	}else if(I.getBit(112) == 1 && WeAreAdding2Denormalize == false ){
		
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
			if(Mukq == 0){
				break;
			}else{
				I = I << 1;
			}
		}
	}

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
		F.setBit(i,0);
	}

	return F;

}
#include "QInt.h"


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

void convertToComplement(QInt &x);

QInt operator+(const QInt &x, const QInt &y){
	QInt kq;
	for (size_t i = 0; i < 128; i++)
	{
		bool nho = 0;
		short int a = x.getBit(i) + y.getBit(i);
		if(a + nho == 2){
			kq.setBit(i, 0);
			nho = 1;
		}else if(a + nho >= 2){
			kq.setBit(i,1);
			nho = 1;
		} else
		{
			kq.setBit(i,a);
			nho = 0;
		}
	}
}

QInt operator-(const QInt &x, const QInt &y){
	QInt kq;
	for (size_t i = 0; i < 128; i++)
	{
		bool nho = 0;
		bool a = x.getBit(i); 
		bool b = y.getBit(i);
		if(a - b - nho == -1){
			kq.setBit(i, 1);
			nho = 1;
		}else if(a - b - nho < -1){
			kq.setBit(i,0);
			nho = 1;
		} else{
			kq.setBit(i,a -b -nho);
			nho = 0;
		}
	}
}

QInt operator*(const QInt &x, const QInt &y){
	
	QInt kq = y;
	// mask dong vai tro nhu M trong thuat toan
	QInt mask = x ;
	bool q_1 = 0; // dong vai tro q-1 
	// dung de xet dau q1, xetdauq2
	bool dauq1  = x.getBit(127)  ;
	bool dauq2 = y.getBit(127);

	// so bit cua multifier sau khi chuan hoa
	unsigned int count = 0;

	//chuan hoa so kq (boi vi dang truoc co the toan la so 1 hoac 0)
	for (size_t i = 127; i >= 0; i--)
	{
		bool a = kq.getBit(i);
		bool b = kq.getBit(i - 1);
		if (a && b)
		{
			if (a)
			{
				kq.setBit(i,0);
			}
		}else
		{
			if(a){ count = i + 1 ;  }
			count  = i;
			break;
		}
	}
	
	//chuan hoa so bi chia (boi vi dang truoc co the toan la so 1 hoac 0)
	for (size_t i = 127; i >= 0; i--)
	{
		bool a = mask.getBit(i);
		bool b = mask.getBit(i - 1);
		if (a && b)
		{
			if (a)
			{
				mask.setBit(i,0);
			}
		}else{
			break;
		}
	}
	// mask co dang 00000multifier00000 (khuc sau co so 0 bang so bits multifier)
	mask << (count);

	for (size_t i = 0; i < count; i++)
	{
		if(q_1 == 0 && kq.getBit(0) == 1){
			kq = kq - mask;
		}else if (q_1 == 1 && kq.getBit(0) == 0)
		{
			kq = kq + mask;
		}
		q_1= kq.getBit(0);
		kq >> 1;
		
	}


	// luc nay ket qua van co the sai, vi can phai dien bit trong vao truoc do nua
	// Neu nhu 2 so khac dau thi dien bit 1 vao truoc do
	if(dauq1 && dauq2 == false){
		for (size_t i = 127; i >= 0; i--)
		{
			if(kq.getBit(i) == false){ 
				kq.setBit(i,1); 
			}else{ break;}
		}
		
	}
	return kq;
	 
}

QInt operator/(const QInt &x, const QInt &y){

	QInt sobichia = x;
	QInt sochia = y;

	//neu co so am -> chuyen ve so khong am
	bool dau1 = sobichia.getBit(127);
	bool dau2 = sochia.getBit(127);
	if(!dau1){
		convertToComplement(sobichia);
	}
	if (!dau2)
	{
		convertToComplement(sochia);
	}
	

	// Danh dau do dai cua 2 so chia va bi chia
	//vd : mark = 5 -> car so chia va so bi chia co do dai la 6 bits
	unsigned int mark; 

	// vua xet xem so bi chia co lon hon so chia khong ? vua xet do dai
	for (size_t i = 127; i >= 0; i--)
	{
		bool a = sobichia.getBit(i);
		bool b = sochia.getBit(i);

		// Neu nhu so bi chia be hon so chia
		if ( a == 0 && b == 1 )
		{
			// tra ve so Qint 0
			return;
		}else if(a == 1){
			mark = i;
		}
	}

	// mark se co dang sochia00000 (so luong bit 0 o phia sau bang so bit cua sobichia)
	QInt mask = sochia;
	mask << mark;
	
	// dung de lap
	unsigned int count = mark;

	for (size_t i = count; i >= 1; i--)
	{
		sobichia << 1;
		sobichia = sobichia - mask;

		// kiem tra xem kq co am khong
		if (sobichia.getBit(127))
		{
			sobichia.setBit(0,0);
			sobichia = sobichia + mask;
			
		}else
		{
			sobichia.setBit(0,1);
		}
	}

	// Neu lay phan sobichia & 0000011111 (so luong bit 1 o phia sau bang so bit cua sobichia, so luong bit 0 bang mark)
	for (size_t i = 127; i > mark ; i--)
	{
		sobichia.setBit(i,0);
	}
	
	// neu 2 so am thi doi dau
	if(!(dau1&&dau2)){
		convertToComplement(sobichia);
	}

	return sobichia;

}

bool operator==(const QInt &x, const QInt &y){
	for (size_t i = 0; i < 128; i++)
	{
		if (x.getBit(i) != y.getBit(i))
		{
			return false;
		}
		
	}
	return true;
	
}

bool operator!=(const QInt &x, const QInt &y){
	for (size_t i = 0; i < 128; i++)
	{
		if (x.getBit(i) == y.getBit(i))
		{
			return false;
		}
		
	}
	return true;
	
}

bool operator>(const QInt &x, const QInt &y){

	bool a = x.getBit(127);
	bool b = y.getBit(127);

	if( a == 1 && b == 0){
		return false;
	}else if(a == 0 && b == 1){
		return true;
	}
	else{

		for (size_t i = 126	; i >= 0; i--)
		{
			if (x.getBit(i) > y.getBit(i))
			{
				return true;
			}else if(x.getBit(i) < y.getBit(i)){
				return false;	
			}
		}
		// xuong toi day nghia la bang nhau
		return false;
			
	}
}

bool operator<(const QInt &x, const QInt &y){

	bool a = x.getBit(127);
	bool b = y.getBit(127);

	if( a == 1 && b == 0){
		return true;
	}else if(a == 0 && b == 1){
		return false;
	}
	else{
		
		for (size_t i = 126	; i >= 0; i--)
		{
			if (x.getBit(i) < y.getBit(i))
			{
				return true;
			}else if(x.getBit(i) > y.getBit(i)){
				return false;	
			}
		}
		// xuong toi day nghia la bang nhau
		return false;
			
	}
}

bool operator<=(const QInt &x, const QInt &y){
	if(x > y){
		return false;
	}
	return true;
}

bool operator>=(const QInt &x, const QInt &y){
	if(x < y){
		return false;
	}
	return true;
}













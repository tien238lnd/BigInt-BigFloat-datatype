#include "QInt.h"


// mình biểu diễn ngược, least thì ở bên trái, most thì ở bên phải
// ví dụ byte i cũng chạy từ 0 bên trái sang 7 bên phải
//  muốn set bit thứ 5 thì phải dịch bit cuối của 00000001 sang trái 3 nấc.
// vậy tức là | ( 1<< (8 - i%8)
// mình biểu diễn vậy thì ngược cách viết ra giấy nhưng mà là thuận theo cách lưu trữ của window chứ không phải ngược đâu
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


void convertToComplement(QInt &x);

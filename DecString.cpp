#include "DecString.h"

DecString::DecString()
{
	for (int i = 0; i < DecString::NUMBYTES; i++)
	{
		bytes[i] = '0';
	}
}
DecString::DecString(std::string src)
{

	char lensrc = char(src.length());
	char thresPoint = DecString::NUMBYTES - 1 - lensrc;

	int i = 0;
	for (; i <= thresPoint; i++) { bytes[i] = '0'; }

	thresPoint++;
	for (; i < DecString::NUMBYTES; i++) { bytes[i] = src[i - thresPoint]; }
}


bool DecString::isZero()
{
	for (int i = 0; i < DecString::NUMBYTES; i++)
	{
		if (bytes[i] != '0') { return false; }
	}

	return true;
}

//return the remainder
bool DecString::dividedBy2()
{
	int i = 0;
	int remainder = 0;
	while (bytes[i] == '0') { i++; }

	while (i < DecString::NUMBYTES)
	{
		int tempval = remainder * 10 + bytes[i] - '0';
		remainder = tempval % 2;
		bytes[i] = tempval / 2 + '0';

		i++;
	}

	return remainder;
}

void DecString::multipliedBy2()
{
	//to quick stop the multiplication
	int stopindex = 0;
	while (bytes[stopindex] == '0') { stopindex++; }
	if (stopindex > 0) { stopindex--; }					//stop at digit before the first digit

	//x2
	int i = DecString::NUMBYTES - 1;
	char remember = 0;
	do
	{
		char newValue = (bytes[i] - '0') * 2 + remember;
		bytes[i] = newValue % 10 + '0';
		remember = newValue / 10;

		i--;
	} while (i >= stopindex);

}

void DecString::added1()
{
	//add 1
	int i = DecString::NUMBYTES - 1;
	char remember = 1;
	do
	{
		char newValue = bytes[i] + remember;
		if (newValue <= '9')
		{
			bytes[i] = newValue;
			break;
		}
		else
		{
			bytes[i] = '0';
		}

		i--;
	} while (i >= 0);
}

std::string DecString::getString()
{
	int i = 0;
	while (bytes[i] == '0') { i++; }

	if (i == DecString::NUMBYTES) { return "0"; }
	return std::string(bytes + i, DecString::NUMBYTES - i);
}
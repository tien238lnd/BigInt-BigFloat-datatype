#include <iostream>
#include <iomanip>
#include <string>
#include "QInt.h"
#include "Qfloat.h"

using namespace std;

std::string lineparseQInt(std::string line);
std::string lineparseQfloat(std::string line);
QInt lineparseQIntGui(std::string line);
Qfloat lineparseQfloatGui(std::string line);
QInt calculate(QInt& operand1, QInt& operand2, string& operatoro);
Qfloat calculate(Qfloat& operand1, Qfloat& operand2, string& operatoro);

void processQInt(std::string& sinput, std::string& soutput);
void processQfloat(std::string& sinput, std::string& soutput);

int main(int argc, char* argv[])
{

	//string input = argv[1];
	//string output = argv[2];
	//string stype = argv[3];
	//int type = stoi(stype);
	//
	///*string input = "E:\\HCMUS\\bachelor\\semester4\\KTMT_HN\\project1\\input.txt";
	//string output = "E:\\HCMUS\\bachelor\\semester4\\KTMT_HN\\project1\\output.txt";
	//int type = 2;*/

	//if (type == 1)
	//{
	//	processQInt(input, output);
	//}
	//else
	//{
	//	processQfloat(input, output);
	//}

	float g = 23e4;
	cout << g << endl;
	Qfloat x = "430000e5";
	cout << x << endl;
	double a = 9876543e301;
	double b = 10;
	double c = a*b;
	double d = -9876543e301 * 10;

	cout << "a = " << a << " : not overflow yet" << endl;
	cout << "c = a * 10 = " << c << endl;
	cout << "c / 0 = " << c / 0 << endl;
	cout << "10 / 0 = " << b / 0 << endl;
	cout << "c + c = " << c + c << endl;
	cout << "c + 0 = " << c + 0 << endl;
	cout << "c - 9a = " << c - 9*a << endl;
	cout << "d = " << d << endl;
	cout << "c + d = " << c + d << endl;
	/*Qfloat x = "0b10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	Qfloat i, f;
	x.modf(i, f);
	cout << x.toBinString() << '\n' << i.toBinString() << '\n' << f.toBinString() << '\n';*/
	//cout << QInt::QInt_MIN() << endl << QInt::QInt_MIN().DectoBin() << endl;

	/*QInt x = "0b11000101111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111000";
	QInt y = x << 2;
	cout << x << endl << x.DectoBin() << endl;
	cout << y << endl << y.DectoBin() << endl;
	QInt z = y >> 2;
	cout << z << endl << z.DectoBin() << endl;*/
	//Qfloat q1;	// 1.25	=> 1.01	=> 1.01e0
	//q1.fromBinString("001111111111111101");
	//cout << q1.toBinString() << endl;
	//Qfloat q2; // 2.5	=> 10.1 => 1.01e1
	//q2.fromBinString("010000000000000001");
	//cout << q2.toBinString() << endl;
	//Qfloat q = q1 + q2;	// 3.75 => 11.11 => 1.111e1
	//cout << q.toBinString();

	// Test 1
	x = "0b0100000000000101111011"; // 1.111011e6 => 1111011 => 123
	cout << x << endl;
	Qfloat y = "0b00111111111111101";	// 1.1e-1  => 0.11 => 0.75
	cout << y << endl;
	Qfloat z = x + y;	// 123.75 => 1111011.11 => 1.11101111e6 => 010000000000010111101111
	std::cout << "7654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	z = x + y;	// 123.75 => 1111011.11 => 1.11101111e6 => 010000000000010 11101111
	cout << z << endl;

	// Test 2
	x = "0b1100000000000000101"; // -1.101e-1 => -3.25
	//cout << x.toBinString() << endl;
	y = "0b0100000000000001101";	// 1.101e2  => 6.5
	//cout << y.toBinString() << endl;
	z = x + y;	// 0 1000...0 101
	std::cout << "7654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	
	cout << x << endl;
	cout << y << endl;
	cout << z << endl;

	//  Test 3
	x = "0b11000000000001100001001001"; // -1.0001001001e7 => -137.425
	//cout << x.toBinString() << endl;
	y = "0b1100000000000000101011";	// -1.101011e1  => -2.34375
	//cout << y.toBinString() << endl;
	z = x + y;	// 1 1000...0110 000110001111
	std::cout << "7654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	cout << x << endl;
	cout << y << endl;
	cout << z << endl;

	//// Test 4
	x = "0b0000000000000000001010111"; // 0.169921875e-16382 , signficand 001010111
	//cout << x.toBinString() << endl;
	y  = "0b0000000000000000010111011";	// 0.365234375e-16382  , signficand 010111011
	//cout << y.toBinString() << endl;
	z = x + y;	// 100010010
	std::cout << "7654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	cout << x << endl;
	cout << y << endl;
	cout << z << endl;

	////// Test 5 2 so denormalize cung dau
	x = "0b1000000000000000001010111"; // 0.169921875e-16382 , signficand 001010111
	//cout << x.toBinString() << endl;
	y = "0b1000000000000000010111011";	// 0.365234375e-16382  , signficand 010111011
	//cout << y.toBinString() << endl;
	z = x + y;	// 100010010
	std::cout << "7654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	cout << x << endl;
	cout << y << endl;
	cout << z << endl;

	//  Test 6 2 so doi nhau
	x = "0b11000000000001100001001001"; // -1.0001001001e7 => -137.425
	//cout << x.toBinString() << endl;
	y = "0b01000000000001100001001001";	// -1.101011e1  => 137.425
	//cout << y.toBinString() << endl;
	z = x + y;	// 0
	std::cout << "7654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	cout << x << endl;
	cout << y << endl;
	cout << z << endl;

	// Test 7 , so rat lon voi so rat be 
	x = "0b110000010010101110"; // -1.1e300 
	//cout << x.toBinString() << endl;
	y = "0b010000000000000001";	// 1.01e1
	//cout << y.toBinString() << endl;
	z = x + y;	// 0
	std::cout << "7654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	cout << x << endl;
	cout << y << endl;
	cout << z << endl;

	cout << "test 7" << endl;
	//////// Test 8 2 so denormalize trai dau
	x = "0b1000000000000000010111"; // -0.359375e-16382 , signficand 001010111
	//cout << x.toBinString() << endl;
	y = "0b000000000000000001011001011";	// 0.3491210938e-16382  , signficand 010111011
	//cout << y.toBinString() << endl;
	z = x + y;	// 10101
	std::cout << "7654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	cout << x << endl;
	cout << y << endl;
	cout << z << endl;

	// Test 9 2 so normal cung dau, tuy nhienn sau khi cong thi phai thay doi mux
	x = "0b0100000000000010011111"; // 1.011111e3 
	//cout << x.toBinString() << endl;
	y = "0b010000000000000111101";	//1.11101e2 
	//cout << y.toBinString() << endl;
	z = x + y;	// 1.00111e4
	std::cout << "7654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	cout << x << endl;
	cout << y << endl;
	cout << z << endl;

	// Test 10 2 so denormal cung dau, tuy nhienn sau khi cong thi phai thay doi mux -> ket qua la normal
	x = "0b1000000000000000111110111"; // 0.169921875e-16382 , signficand 111110111
	//cout << x.toBinString() << endl;
	y = "0b1000000000000000110111011";	// 0.365234375e-16382  , signficand 110111011
	//cout << y.toBinString() << endl;
	z = x + y;	// 1.11011 e4
	std::cout << "7654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	cout << x << endl;
	cout << y << endl;
	cout << z << endl;
	cout << "test 11" << endl;
	// Test 11 2 so normal trai dau, tuy nhienn sau khi cong thi phai thay doi mux , cu the la phai giam mu 
	x = "0b01000000000000010111001"; // 1.0111001 e2 , signficand 111110111
	//cout << x.toBinString() << endl;
	y = "0b1100000000000001011111";	// -1.011111e2  , signficand 110111011
	//cout << y.toBinString() << endl;
	z = x + y;	// 
	std::cout << "7654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	cout << x << endl;
	cout << y << endl;
	cout << z << endl;

	// Test 12 so dac biet
	x = "0b0111111111111111101100"; // infini
	//cout << x.toBinString() << endl;
	y = "0b0111111111111111101100";	// infini
	//cout << y.toBinString() << endl;
	z = x + y;	// 
	std::cout << "7654321|987654321|987654321|987654321|98765	4321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|987654321|" << std::endl;
	cout << x << endl;
	cout << y << endl;
	cout << z << endl;

	cout << "Done!" << endl;
	return 0;
}

void processQInt(std::string& sinput, std::string& soutput)
{
	//open file
	ifstream input(sinput);
	if (!input.is_open()) { return; }
	ofstream output(soutput);
	if (!output.is_open()) { input.close(); return; }

	std::string line;

	//first line
	if (getline(input, line) && line.length() != 0)
	{
		output << lineparseQInt(line);
	}
	//process every line after
	while (!input.eof())
	{
		if (getline(input, line) && line.length()!=0)
		{
			
			output << endl << lineparseQInt(line);
		}
	}

	//close file
	input.close();
	output.close();
}

void processQfloat(std::string& sinput, std::string& soutput)
{
	//open file
	ifstream input(sinput);
	if (!input.is_open()) { return; }
	ofstream output(soutput);
	if (!output.is_open()) { input.close(); return; }

	std::string line;

	//first line
	if (getline(input, line) && line.length() != 0)
	{
		output << lineparseQfloat(line);
	}
	//process every line after
	while (!input.eof())
	{
		if (getline(input, line) && line.length() != 0)
		{
			output << endl << lineparseQfloat(line);
		}
	}

	//close file
	input.close();
	output.close();
}

std::string lineparseQInt(std::string line)
{
	std::string firstStr;
	std::string secondStr;
	std::string thirdStr;
	std::string fourthStr;
	int linelength = line.length();
	int i = 0;

	////////////////////
	//parse
	////////////////////
	//base or the first base
	while (line[i] != ' ')
	{
		firstStr += line[i];
		i++;
	}
	while (line[i] == ' ') { i++; }

	//first operand or second base or ~ operator
	do
	{
		secondStr += line[i];
		i++;
	} while ((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'));
	
	while (line[i] == ' ') { i++; }

	bool third_is_operator = false;
	//operator or the number to convert
	if (secondStr[0] == '~')
	{
		do
		{
			thirdStr += line[i];
			i++;
		} while (i < linelength && ((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')));

	}
	else
	{
		thirdStr += line[i];
		i++;

		if (i >= linelength)
		{//do nothing, just use to pass the following branches if face this case
		}
		//third string is a number
		else if ((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'))
		{
			do
			{
				thirdStr += line[i];
				i++;
			} while (i < linelength && ((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')));
		}
		else //third string is a operator
		{
			third_is_operator = true;
			while (i<linelength && line[i] != ' ')
			{
				thirdStr += line[i];
				i++;
			}
		}
	}

	while (i<linelength && line[i] == ' ') { i++; }
	//may or may not be the second operand
	if (third_is_operator)
	{
		do
		{
			fourthStr += line[i];
			i++;
		} while (i < linelength && ((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')));
	}

	////////////////////
	//process
	////////////////////
	int firstbase = stoi(firstStr);
	int resultbase = firstbase;
	QInt qiResult;
	
	if (secondStr[0] == '~')//operator not
	{
		if (resultbase == 2) { thirdStr = "0b" + thirdStr; }
		else if (resultbase == 16) { thirdStr = "0x" + thirdStr; }

		qiResult = thirdStr;
		qiResult = ~qiResult;
	}
	else if (third_is_operator)//calculate
	{
		if (resultbase == 2)
		{
			secondStr = "0b" + secondStr;
			fourthStr = "0b" + fourthStr;
		}
		else if (resultbase == 16)
		{
			secondStr = "0x" + secondStr;
			fourthStr = "0x" + fourthStr;
		}
		
		QInt operand1(secondStr);
		QInt operand2(fourthStr);

		qiResult = calculate(operand1, operand2, thirdStr);

	}
	else//convert between bases
	{
		resultbase = stoi(secondStr);
		if (firstbase == 2) { thirdStr = "0b" + thirdStr; }
		else if (firstbase == 16) { thirdStr = "0x" + thirdStr; }

		qiResult = thirdStr;
	}

	if (resultbase == 2)
	{
		return qiResult.DectoBin(); 
	}
	else if (resultbase == 10) 
	{ 
		return qiResult.DecToDecString(); 
	}
	else {
		return qiResult.DectoHex();
	}
}
std::string lineparseQfloat(std::string line)
{
	std::string firstStr;
	std::string secondStr;
	std::string thirdStr;
	std::string fourthStr;
	int linelength = line.length();
	int i = 0;

	////////////////////
	//parse
	////////////////////
	//base or the first base
	while (line[i] != ' ')
	{
		firstStr += line[i];
		i++;
	}
	while (line[i] == ' ') { i++; }

	//first operand or second base
	do
	{
		secondStr += line[i];
		i++;
	} while (line[i] == '.' || line[i]=='e' || line[i]== '-' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'));

	while (line[i] == ' ') { i++; }

	bool third_is_operator = false;
	//operator or the number to convert
	thirdStr += line[i];
	i++;

	if (i >= linelength)
	{//do nothing, just use to pass the following branches if face this case
	}
	//third string is a number
	else if (line[i] == '.' || line[i] == 'e' || line[i] == '-' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'))
	{
		do
		{
			thirdStr += line[i];
			i++;
		} while (i < linelength && (line[i] == '.' || line[i] == 'e' || line[i] == '-' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')));
	}
	else //third string is a operator
	{
		third_is_operator = true;
		while (i < linelength && line[i] != ' ')
		{
			thirdStr += line[i];
			i++;
		}
	}

	while (i < linelength && line[i] == ' ') { i++; }
	//may or may not be the second operand
	if (third_is_operator)
	{
		do
		{
			fourthStr += line[i];
			i++;
		} while (i < linelength && (line[i] == '.' || line[i] == 'e' || line[i] == '-' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')));
	}

	////////////////////
	//process
	////////////////////
	int firstbase = stoi(firstStr);
	int resultbase = firstbase;
	Qfloat qfResult;

	if (third_is_operator)//calculate
	{
		if (resultbase == 2)
		{
			secondStr = "0b" + secondStr;
			fourthStr = "0b" + fourthStr;
		}

		Qfloat operand1(secondStr);
		Qfloat operand2(fourthStr);

		qfResult = calculate(operand1, operand2, thirdStr);
	}
	else//convert between bases
	{
		resultbase = stoi(secondStr);
		if (firstbase == 2) { thirdStr = "0b" + thirdStr; }

		qfResult = thirdStr;
	}

	if (resultbase == 2)
	{
		return qfResult.toBinString();
	}
	else
	{
		return qfResult.toDecString();
	}
}


QInt lineparseQIntGui(std::string line)
{
	std::string firstStr;
	std::string secondStr;
	std::string thirdStr;
	std::string fourthStr;
	int linelength = line.length();
	int i = 0;

	////////////////////
	//parse
	////////////////////
	//base
	while (line[i] != ' ')
	{
		firstStr += line[i];
		i++;
	}
	while (line[i] == ' ') { i++; }

	//first operand or the number to convert or ~ operator
	do
	{
		secondStr += line[i];
		i++;
	} while ((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'));

	while (i < linelength && line[i] == ' ') { i++; }

	bool third_is_operator = false;
	//operator or the number to convert
	if (secondStr[0] == '~')
	{
		do
		{
			thirdStr += line[i];
			i++;
		} while (i < linelength && ((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')));

	}
	else if(i < linelength)
	{
		thirdStr += line[i];
		i++;

		if (i >= linelength)
		{//do nothing, just use to pass the following branches if face this case
		}
		//third string is a number
		else if ((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'))
		{
			do
			{
				thirdStr += line[i];
				i++;
			} while (i < linelength && ((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')));
		}
		else //third string is a operator
		{
			third_is_operator = true;
			while (i < linelength && line[i] != ' ')
			{
				thirdStr += line[i];
				i++;
			}
		}
	}

	while (i < linelength && line[i] == ' ') { i++; }
	//may or may not be the second operand
	if (third_is_operator)
	{
		while (i < linelength && ((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')))
		{
			fourthStr += line[i];
			i++;
		} 
	}

	////////////////////
	//process
	////////////////////
	int firstbase = stoi(firstStr);
	int resultbase = firstbase;
	QInt qiResult;

	if (secondStr[0] == '~')//operator not
	{
		if (resultbase == 2) { thirdStr = "0b" + thirdStr; }
		else if (resultbase == 16) { thirdStr = "0x" + thirdStr; }

		qiResult = thirdStr;
		qiResult = ~qiResult;
	}
	else if (third_is_operator)//calculate
	{
		if (fourthStr.length() == 0) { fourthStr += '0'; }
		if (resultbase == 2)
		{
			secondStr = "0b" + secondStr;
			fourthStr = "0b" + fourthStr;
		}
		else if (resultbase == 16)
		{
			secondStr = "0x" + secondStr;
			fourthStr = "0x" + fourthStr;
		}

		QInt operand1(secondStr);
		QInt operand2(fourthStr);

		qiResult = calculate(operand1, operand2, thirdStr);

	}
	else//convert between bases
	{
		if (firstbase == 2) { secondStr = "0b" + secondStr; }
		else if (firstbase == 16) { secondStr = "0x" + secondStr; }

		qiResult = secondStr;
	}

	return qiResult;
}

Qfloat lineparseQfloatGui(std::string line)
{

	std::string firstStr;
	std::string secondStr;
	std::string thirdStr;
	std::string fourthStr;
	int linelength = line.length();
	int i = 0;

	////////////////////
	//parse
	////////////////////
	//base
	while (line[i] != ' ')
	{
		firstStr += line[i];
		i++;
	}
	while (line[i] == ' ') { i++; }

	//first operand or number to convert
	do
	{
		secondStr += line[i];
		i++;
	} while (line[i] == '.' || line[i] == 'e' || line[i] == '-' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'));

	while (i<linelength && line[i] == ' ') { i++; }

	bool third_is_operator = false;
	//operator or null, so we must check to avoid error
	if (i < linelength)
	{
		thirdStr += line[i];
		i++;
	}
	
	if (i >= linelength)
	{//do nothing, just use to pass the following branches if face this case
	}
	//third string is a number
	else if (line[i] == '.' || line[i] == 'e' || line[i] == '-' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'))
	{
		do
		{
			thirdStr += line[i];
			i++;
		} while (i < linelength && (line[i] == '.' || line[i] == 'e' || line[i] == '-' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')));
	}
	else //third string is a operator
	{
		third_is_operator = true;
		while (i < linelength && line[i] != ' ')
		{
			thirdStr += line[i];
			i++;
		}
	}

	while (i < linelength && line[i] == ' ') { i++; }
	//may or may not be the second operand
	if (third_is_operator)
	{
		while (i < linelength && (line[i] == '.' || line[i] == 'e' || line[i] == '-' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')))
		{
			fourthStr += line[i];
			i++;
		}
	}

	////////////////////
	//process
	////////////////////
	int firstbase = stoi(firstStr);
	int resultbase = firstbase;
	Qfloat qfResult;

	if (third_is_operator)//calculate
	{
		if (fourthStr.length() == 0) { fourthStr += '0'; }
		if (resultbase == 2)
		{
			secondStr = "0b" + secondStr;
			fourthStr = "0b" + fourthStr;
		}

		Qfloat operand1(secondStr);
		Qfloat operand2(fourthStr);

		qfResult = calculate(operand1, operand2, thirdStr);
	}
	else//convert between bases
	{
		if (firstbase == 2) { secondStr = "0b" + secondStr; }

		qfResult = secondStr;
	}

	return qfResult;
}

QInt calculate(QInt& operand1, QInt& operand2, string& operatoro)
{
	if (operatoro.compare("<<")==0)
	{
		return operand1 << operand2.castToInt();
	}
	else if (operatoro.compare(">>")==0)
	{
		return operand1 >> operand2.castToInt();
	}
	else if(operatoro.compare("rol")==0)
	{
		return rol(operand1, operand2.castToInt());
	}
	else if (operatoro.compare("ror")==0)
	{
		return ror(operand1, operand2.castToInt());
	}
	else
	{
		switch (operatoro[0])
		{
		case '+':
			return operand1 + operand2;
		case '-':
			return operand1 - operand2;
		case '*':
			return operand1 * operand2;
		case '/':
			return operand1 / operand2;
		case '&':
			return operand1 & operand2;
		case '|':
			return operand1 | operand2;
		case '^':
			return operand1 ^ operand2;
		case '>':
			if (operand1 > operand2) { return "1"; }
			else return "0";
		case '<':
			if (operand1 < operand2) { return "1"; }
			else return "0";
		case '==':
			if (operand1 == operand2) { return "1"; }
			else return "0";
		case '!=':
			if (operand1 != operand2) { return "1"; }
			else return "0";
		case '>=':
			if (operand1 >= operand2) { return "1"; }
			else return "0";
		case '<=':
			if (operand1 <= operand2) { return "1"; }
			else return "0";
		}
	}
	return "0";
}


Qfloat calculate(Qfloat& operand1, Qfloat& operand2, string& operatoro)
{
	switch (operatoro[0])
	{
	case '+':
		return operand1 + operand2;
	case '-':
		return operand1 - operand2;
	case '*':
		return operand1 * operand2;
	case '/':
		return operand1 / operand2;
	}
	return "0";
}

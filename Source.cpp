#include <iostream>
#include <iomanip>
#include <string>
#include "QInt.h"
#include "Qfloat.h"
using namespace std;

std::string lineparseQInt(std::string line);
QInt calculate(QInt& operand1, QInt& operand2, string& operatoro);
void processQInt(std::string& sinput, std::string& soutput);
void processQfloat(std::string& sinput, std::string& soutput);

//int main(int argc, char* argv[])
//{
//
//	string input = argv[1];
//	string output = argv[2];
//	string stype = argv[3];
//	int type = stoi(stype);
//
//	if (type == 1)
//	{
//		processQInt(input, output);
//	}
//	else
//	{
//		processQfloat(input, output);
//	}
//
//	QInt a("110111010000111011");
//	cout << a.DectoBin() << endl;
//	QInt b = rol(a, 2);
//	cout << b << endl;
//	cout << (rol(a, 2)).DectoBin() << endl;
//
//	cout << "Done! Cheer!" << endl;
//	return 0;
//}

int main()
{
	/*double a = 9876543e301;
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
	cout << "c + d = " << c + d << endl;*/
	Qfloat x = "0b10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	Qfloat i, f;
	x.modf(i, f);
	cout << x.toBinString() << '\n' << i.toBinString() << '\n' << f.toBinString() << '\n';
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

	//Qfloat x = "0b0100000000000101111011"; // 1.111011e6 => 1111011 => 123
	//cout << x.toBinString() << endl;
	//Qfloat y = "0b00111111111111101";	// 1.1e-1  => 0.11 => 0.75
	//cout << y.toBinString() << endl;
	//Qfloat z = x + y;	// 123.75 => 1111011.11 => 1.11101111e6 => 010000000000010111101111
	//cout << z.toBinString();

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
	int i = 0;
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
		} while (i < linelength && (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'));

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
			} while (i < linelength && (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'));
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
		} while (i < linelength && (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'));
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

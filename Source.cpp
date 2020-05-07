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
	Qfloat x;
	x = "0b010000000000001001";
	cout << x.toBinString();
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

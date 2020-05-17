#include <iostream>
#include <string>
#include "QInt.h"
#include "Qfloat.h"

using namespace std;

//QInt input line
//input a line in correct format in command line
//return a string is the result of process that line
std::string lineparseQInt(std::string line);

//Qfloat input line
//input a line in correct format in command line
//return a string is the result of process that line
std::string lineparseQfloat(std::string line);

//QInt input in GUI
//input a line in gui format
//have base of input in front, like "10" (mean 0 in base 10), or "10 5 + 6" or "2 1001"
//return the value QInt of input line
QInt lineparseQIntGui(std::string line);

//Qfloat input in GUI
//input a line in gui format
//have base of input in front, like "10" (mean 0 in base 10), or "10 5.0 + 6" or "2 1001.01101", or "10 -5e-8"
//return the value Qfloat of input line
Qfloat lineparseQfloatGui(std::string line);

//calculate expression: operand1 operatoro operand 2 (ex: 5 + 6, 2 >> 4, 5 rol 1...)
QInt calculate(QInt& operand1, QInt& operand2, string& operatoro);

//calculate expression: operand1 operatoro operand 2 (ex: 5.0 + 6e4)
Qfloat calculate(Qfloat& operand1, Qfloat& operand2, string& operatoro);

//with every line in input file (which path is sinput)
//process the line and print it to output file (which path is soutput)
void processQInt(std::string& sinput, std::string& soutput);

//with every line in input file (which path is sinput)
//process the line and print it to output file (which path is soutput)
void processQfloat(std::string& sinput, std::string& soutput);

int main(int argc, char* argv[])
{
	string input = argv[1];
	string output = argv[2];
	string stype = argv[3];
	int type = stoi(stype);

	if (type == 1)
	{
		processQInt(input, output);
	}
	else
	{
		processQfloat(input, output);
	}

	cout << "Done!" << endl;
	return 0;
}

void processQInt(std::string& sinput, std::string& soutput)
{
	//open file
	ifstream input(sinput);
	if (!input.is_open()) 
	{
		cout << "Can't open input file" << endl;
		return;
	}
	ofstream output(soutput);
	if (!output.is_open())
	{
		cout << "Can't open output file" << endl;
		input.close(); return;
	}

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
	if (!input.is_open()) 
	{
		cout << "Can't open input file" << endl;
		return;
	}
	ofstream output(soutput);
	if (!output.is_open())
	{
		cout << "Can't open output file" << endl;
		input.close();
		return;
	}

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

	//first operand (include case '-' in the first position) or second base or ~ operator
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
		do//number to convert (include case '-' in the first position)
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
			do// (include case '-' in the first position)
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
		do//(include case '-' in the first position)
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
	} while (line[i] == '.' || line[i]=='e' || line[i]== '-' || line[i] == '+' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'));

	while (line[i] == ' ') { i++; }

	bool third_is_operator = false;
	//operator or the number to convert
	thirdStr += line[i];
	i++;

	if (i >= linelength)
	{//do nothing, just use to pass the following branches if face this case
	}
	//third string is a number
	else if (line[i] == '.' || line[i] == 'e' || line[i] == '-' || line[i] == '+' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'))
	{
		do
		{
			thirdStr += line[i];
			i++;
		} while (i < linelength && (line[i] == '.' || line[i] == 'e' || line[i] == '-' || line[i] == '+' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')));
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
		} while (i < linelength && (line[i] == '.' || line[i] == 'e' || line[i] == '-'|| line[i]=='+' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')));
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
	//preprocess line
	int ik = 1;
	while (ik < int(line.length()))
	{
		if (strchr("+-*/&|^~", line[ik]) != nullptr && strchr("0123456789ABCDEF", line[ik - 1]))
		{
			line.insert(ik, " ");
			line.insert(ik + 2, " ");
			ik += 3;
		}
		else if ((line[ik] == '>' && line[ik] == '>') || (line[ik] == '<' && line[ik] == '<'))
		{
			line.insert(ik - 1, " ");
			line.insert(ik + 2, " ");
			ik += 3;
		}
		else ik++;
	}
	

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

	while (i<linelength && line[i] == ' ') { i++; }
	if (i >= linelength) { return QInt("0"); }
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
		else if (line[i] == '-' || line[i] == '+' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'))
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
		while (i < linelength && (line[i]=='-' || line[i] == '+'|| (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')))
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
	//preprocess line
	int ik = 1;
	while (ik < int(line.length()))
	{
		if (strchr("+-*/&|^~", line[ik]) != nullptr && strchr("0123456789ABCDEF", line[ik - 1]))
		{
			line.insert(ik, " ");
			line.insert(ik + 2, " ");
			ik += 3;
		}
		else if ((line[ik] == '>' && line[ik] == '>') || (line[ik] == '<' && line[ik] == '<'))
		{
			line.insert(ik - 1, " ");
			line.insert(ik + 2, " ");
			ik += 3;
		}
		else ik++;
	}

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
	while (i<linelength && line[i] == ' ') { i++; }
	if (i >= linelength) { return Qfloat("0"); }

	//first operand or number to convert
	do
	{
		secondStr += line[i];
		i++;
	} while (line[i] == '.' || line[i] == 'e' || line[i] == '-' || line[i] == '+' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'));

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
	else if (line[i] == '.' || line[i] == 'e' || line[i] == '-' || line[i] == '+' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f'))
	{
		do
		{
			thirdStr += line[i];
			i++;
		} while (i < linelength && (line[i] == '.' || line[i] == 'e' || line[i] == '-' || line[i] == '+' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')));
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
		while (i < linelength && (line[i] == '.' || line[i] == 'e' || line[i] == '-' || line[i] == '+' || (line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'F') || (line[i] >= 'a' && line[i] <= 'f')))
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
			if(1==1){}
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

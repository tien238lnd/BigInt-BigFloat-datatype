#include <iostream>
#include <iomanip>
#include "QInt.h"
#include <string>
using namespace std;




int main(int argc, char* argv)
{
	QInt qi("30");
	QInt qb("20");
	//qi = QInt("-15");
	//qb = "22";
	cout << qi / qb << endl;

	std::string a;
	return 0;
}
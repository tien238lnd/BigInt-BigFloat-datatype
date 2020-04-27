#pragma once


static const int numbytes = 39;
char bytes[numbytes];		// "123456789012345678901234567890"

DecString();
DecString(const DecString&);
DecString(const char*);


char getAt(char index);
void setAt(char index, char value);

//DecString& operator=(const DecString& other);
//DecString& operator=(const char* other);

bool checkNegative();
bool checkZero();
void convertToPositive();

char devidedBy2();
bool multipliedBy2();
bool added1();



class Qfloat{
    char bytes[16];
    
public:
    
    Qfloat();
    // ~Qfloat();

private:
	//set the bit at position i the value b
	//i=0 associate with LSB
	//i=127 associate with MSB
	void setBit(char i, bool b);

	//get the value of the bit at position i
	bool getBit(char i) const;

	//convert to 2's complement of this number
	// void convertTo2sComplement();

public:
    // yeye
    // Tien
	//default constructor, value is Zero
	Qfloat();

	//Copy constructor
	Qfloat(const QInt& src);

	//constructor with string
	Qfloat(std::string src);

	//constructor with const char*
	Qfloat(const char* src);

	//assign operator
	Qfloat& operator=(const Qfloat& src);

	//assign operator, to a value descript by a string
	//operator accept value in both decimal
	//for negative decimal: have "-" sign in the front
	//for binary: srcStr have "0b" in the front
	Qfloat& operator=(std::string srcStr);

	//assign operator, to a value descript by a const char
	//operator accept value in both decimal
	//for negative decimal: have "-" sign in the front
	//for binary: srcStr have "0b" in the front
	Qfloat& operator=(const char* srcStr);

    //input a Qfloat qi from istream istr
    friend std::istream& operator>>(std::istream& istr, Qfloat& qi);

    //output a Qfloat qi to the ostream ostr
    friend std::ostream& operator<<(std::ostream& ostr, const Qfloat& qi);

// ----------------------------------------------------------

    // hieu

    friend Qfloat operator+(const Qfloat &x, const Qfloat &y);
    friend Qfloat operator-(const Qfloat &x, const Qfloat &y);
    friend Qfloat operator*(const Qfloat &x, const Qfloat &y);
    friend Qfloat operator/(const Qfloat &x, const Qfloat &y);




};
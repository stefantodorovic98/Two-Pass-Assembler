#ifndef OUTPUT_RELOC_H
#define OUTPUT_RELOC_H

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

class OutputReloc
{
private:
	int offset;
	string type;
	int value;
	string data_instr;
public:
	OutputReloc() :
		offset(0), type(""), value(0), data_instr("")
	{	}
	OutputReloc(int offset, string type, int value, string data_instr) : 
		offset(offset), type(type), value(value), data_instr(data_instr)
	{	}

	void printOutputReloc(ofstream& outputFile);
	void printOutputReloc();

	void writeBinary(ofstream& outputBinary);
	void readBinary(ifstream& inputBinary);

};

#endif
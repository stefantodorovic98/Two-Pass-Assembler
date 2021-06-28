#ifndef OUTPUT_SECTION_H
#define OUTPUT_SECTION_H

#include <iostream>
#include <vector>
#include <fstream>

#include "OutputReloc.h"

using namespace std;

class OutputSection
{
private:
	string name;
	int ordinal;
	int lc = 0;
	vector<string> byteData;
	vector<OutputReloc> relocVector;
public:
	OutputSection() :
		name(""), ordinal(0)
	{	}
	OutputSection(string name, int ordinal) :
		name(name), ordinal(ordinal)
	{	}
	
	string getName();
	void printByteData(ofstream& outputFile);
	void printByteData();
	void printRelocData(ofstream& outputFile);
	void printRelocData();
	void insertByteData(string byte);
	void insertRelocData(string type, int value, string data_instr);

	int getOrdinal();
	int getLocalCounter();

	void writeBinary(ofstream& outputBinary);
	void readBinary(ifstream& inputBinary);
};

#endif


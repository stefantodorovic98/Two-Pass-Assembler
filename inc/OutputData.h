#ifndef OUTPUT_DATA_H
#define OUTPUT_DATA_H

#include <iostream>
#include <vector>
#include <fstream>

#include "OutputSection.h"

using namespace std;

class OutputData
{
private:
	vector<OutputSection> dataList;
public:
	void insertSection(OutputSection outputSection);
	void insertRelocData(int currSection, string type, int value, string data_instr);
	bool sectionExists(string name, int& currSection);
	void insertByte(int currSection, string byte);
	void insertShort(int currSection, string lowByte, string highByte);
	void insertSkipBytes(int currSection, short int num);
	void printByteData(ofstream& outputFile);
	void printByteData();
	void printRelocData(ofstream& outputFile);
	void printRelocData();

	bool getLocalCounter(int currSection, int& lc);

	void writeBinary(ofstream& outputBinary);
	void readBinary(ifstream& inputBinary);
};

#endif


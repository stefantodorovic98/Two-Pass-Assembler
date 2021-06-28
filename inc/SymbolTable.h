#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <iomanip>

#include "Row.h"

using namespace std;

class SymbolTable
{
private:
	vector<Row> symbolVector;
public:
	void insertSymbol(Row row);
	void print(ofstream& outputFile);
	void print();
	bool existsSection(string sectionName, int& ordinal, int& sectionSize);
	void setSectionSize(int ordinal, int sectionSize);
	bool existsLabel(string labelName);
	bool alreadyWasGlobal(string labelName);
	bool checkForGlobal(string labelName);
	bool checkDefinedExternGlobal(string labelName);
	bool defineLabel(string labelName, int section, int value);
	bool labelExtern(string labelName);
	bool undefinedGlobal();
	void rearrangeData();

	int getOrdinal(string name);
	bool getRow(string name, Row& row);

	bool isLocal(string name, int& value, int& section);
	bool isGlobal(string name, int& value, int& ordinal, int& section);
	bool isAbsolute(string name, int& value, int& ordinal);

	void writeBinary(ofstream& outputBinary);
	void readBinary(ifstream& inputBinary);

	bool globalSection(string sectionName, int section);
};

#endif


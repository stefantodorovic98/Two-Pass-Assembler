#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "SymbolTable.h"
#include "RegularExpressions.h"
#include "OutputData.h"
#include "ServiceClass.h"

using namespace std;

class Assembler
{
private:
	int lineNumber = 0;
	int lc = 0;
public:
	static string trimString(string arg);
	void parseLineWithOneReg(vector<string>& oneLine, string operation, string line);
	void parseLineWithTwoRegs(vector<string>& oneLine, string operation, string line);
	void parseLineWithOperand(vector<string>& oneLine, string operation, string line);
	void parseLineWithRegOperand(vector<string>& oneLine, string operation, string line);

	void FirstPass(ifstream& inputFile, ofstream& outputFile, SymbolTable& symbolTable, vector<vector<string>>& parsedCode, bool& condition);
	void SecondPass(ofstream& outputFile, SymbolTable& symbolTable, vector<vector<string>>& parsedCode, OutputData& outputData, bool& condition);
};

#endif


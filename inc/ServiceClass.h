#ifndef SERVICE_CLASS_H
#define SERVICE_CLASS_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <regex>
#include <string>

#include "OutputData.h"
#include "RegularExpressions.h"
#include "SymbolTable.h"
#include "Assembler.h"

using namespace std;

class ServiceClass
{
public:
	static void writeNoRegisterJmp(int currSection, OutputData& outputData);
	static void writeRegisterJmp(int currSection, string regName,OutputData& outputData);
	static void writeRegister(int currSection, string regName, OutputData& outputData);
	static void writeTwoRegisters(int currSection, string regName1, string regName2, OutputData& outputData);
	static void writeInstructionWithOneReg(int currSection, short int instruction, string regName, OutputData& outputData);
	static void writeInstuctionWithTwoRegs(int currSection, short int instruction, string regName1, string regName2, OutputData& outputData);
	static void writeByte(int currSection, short int num, OutputData& outputData);
	static void writeShortInstr(int currSection, short int num, OutputData& outputData);
	static void writeShortData(int currSection, short int num, OutputData& outputData);
	static void writeSkipBytes(int currSection, short int num, OutputData& outputData);

	static void writeSymbolAbsAddrInstr(SymbolTable& symbolTable, int currSection, string symbol, OutputData& outputData);
	static void writeSymbolAbsAddrData(SymbolTable& symbolTable, int currSection, string symbol, OutputData& outputData);
	static void writeSymbolRelAddrInstr(SymbolTable& symbolTable, int currSection, string symbol, OutputData& outputData);

	static void writeDataInstructions(SymbolTable& symbolTable, int currSection, short int instruction, bool& condition, int& lineNumber,
		string regName1, string operandName, OutputData& outputData);
	static void writePushPop(int currSection, short int instruction, short int addrMode, string regName1, OutputData& outputData);

	static void writeJumpInstructions(SymbolTable& symbolTable, int currSection, short int instruction, bool& condition, int& lineNumber,
		string operandName, OutputData& outputData);

};

#endif


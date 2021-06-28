#include "ServiceClass.h"

void ServiceClass::writeNoRegisterJmp(int currSection, OutputData& outputData)
{
    short int num = 0xFF;
    ServiceClass::writeByte(currSection, num, outputData);
}

void ServiceClass::writeRegisterJmp(int currSection, string regName, OutputData& outputData)
{
    stringstream ss;
    short int num = 0;
    if (regex_match(regName, RegularExpressions::REGISTER_0_7))
    {
        char regNum = regName[1];
        num = regNum - '0';
    }
    else
    {
        if (regName == "sp")
        {
            num = '6' - '0';
        }
        else if (regName == "pc")
        {
            num = '7' - '0';
        }
        else if (regName == "psw")
        {
            num = '8' - '0';
        }
    }
    num = (num & 0x0F) | 0xF0;
    ss << setfill('0') << setw(2) << right << hex << num;
    string byte(ss.str());
    outputData.insertByte(currSection, byte);
}

void ServiceClass::writeRegister(int currSection, string regName, OutputData& outputData)
{
    stringstream ss;
    short int num = 0;
    if (regex_match(regName, RegularExpressions::REGISTER_0_7))
    {
        char regNum = regName[1];
        num = regNum - '0';
    }
    else
    {
        if (regName == "sp")
        {
            num = '6' - '0';
        }
        else if (regName == "pc")
        {
            num = '7' - '0';
        }
        else if (regName == "psw")
        {
            num = '8' - '0';
        }
    }
    num = ((num << 4) & 0xF0) | 0x0F;
    ss << setfill('0') << setw(2) << right << hex << num;
    string byte(ss.str());
    outputData.insertByte(currSection, byte);
}

void ServiceClass::writeTwoRegisters(int currSection, string regName1, string regName2, OutputData& outputData)
{
    stringstream ss;
    short int num1 = 0;
    if (regex_match(regName1, RegularExpressions::REGISTER_0_7))
    {
        char regNum = regName1[1];
        num1 = regNum - '0';
    }
    else
    {
        if (regName1 == "sp")
        {
            num1 = '6' - '0';
        }
        else if (regName1 == "pc")
        {
            num1 = '7' - '0';
        }
        else if (regName1 == "psw")
        {
            num1 = '8' - '0';
        }
    }

    short int num2 = 0;

    if (regex_match(regName2, RegularExpressions::REGISTER_0_7))
    {
        char regNum = regName2[1];
        num2 = regNum - '0';
    }
    else
    {
        if (regName2 == "sp")
        {
            num2 = '6' - '0';
        }
        else if (regName2 == "pc")
        {
            num2 = '7' - '0';
        }
        else if (regName2 == "psw")
        {
            num2 = '8' - '0';
        }
    }

    short int num = ((num1 << 4) & 0xF0) | (num2 & 0x0F);;

    ss << setfill('0') << setw(2) << right << hex << num;
    string byte(ss.str());
    outputData.insertByte(currSection, byte);
}

void ServiceClass::writeInstructionWithOneReg(int currSection, short int instruction, string regName, OutputData& outputData)
{
    ServiceClass::writeByte(currSection, instruction, outputData);
    ServiceClass::writeRegister(currSection, regName, outputData);
}

void ServiceClass::writeInstuctionWithTwoRegs(int currSection, short int instruction, string regName1, string regName2, OutputData& outputData)
{
    ServiceClass::writeByte(currSection, instruction, outputData);
    ServiceClass::writeTwoRegisters(currSection, regName1, regName2, outputData);
}

void ServiceClass::writeByte(int currSection, short int num, OutputData& outputData)
{
    stringstream ss;
    short int low = num & 0x00FF;
    ss << setfill('0') << setw(2) << right << hex << low;
    string lowByte(ss.str());
    outputData.insertByte(currSection, lowByte);
}

void ServiceClass::writeShortInstr(int currSection, short int num, OutputData& outputData)
{
    stringstream ss;
    short int low = num & 0x00FF;
    ss << setfill('0') << setw(2) << right << hex << low;
    string lowByte(ss.str());

    ss.str(string());
    ss.clear();

    short int high = ((num & 0xFF00) >> 8) & 0x00FF;
    ss << setfill('0') << setw(2) << right << hex << high;
    string highByte(ss.str());

    outputData.insertShort(currSection, highByte, lowByte);
}

void ServiceClass::writeShortData(int currSection, short int num, OutputData& outputData)
{
    stringstream ss;
    short int low = num & 0x00FF;
    ss << setfill('0') << setw(2) << right << hex << low;
    string lowByte(ss.str());

    ss.str(string());
    ss.clear();

    short int high = ((num & 0xFF00) >> 8) & 0x00FF;
    ss << setfill('0') << setw(2) << right << hex << high;
    string highByte(ss.str());

    outputData.insertShort(currSection, lowByte, highByte);
}

void ServiceClass::writeSkipBytes(int currSection, short int num, OutputData& outputData)
{
    outputData.insertSkipBytes(currSection, num);
}

void ServiceClass::writeSymbolAbsAddrInstr(SymbolTable& symbolTable, int currSection, string symbol, OutputData& outputData)
{
    int value = 0;
    int section = 0;
    int ordinal = 0;

    if (symbolTable.isAbsolute(symbol, value, ordinal))
    {
        ServiceClass::writeShortInstr(currSection, value, outputData);
    }
    else if (symbolTable.isLocal(symbol, value, section))
    {
        outputData.insertRelocData(currSection, "R_386_16", section, "instr");
        ServiceClass::writeShortInstr(currSection, value, outputData);
    }
    else if (symbolTable.isGlobal(symbol, value, ordinal, section))
    {
        outputData.insertRelocData(currSection, "R_386_16", ordinal, "instr");
        ServiceClass::writeShortInstr(currSection, 0, outputData);
    }
}

void ServiceClass::writeSymbolAbsAddrData(SymbolTable& symbolTable, int currSection, string symbol, OutputData& outputData)
{
    int value = 0;
    int section = 0;
    int ordinal = 0;

    if (symbolTable.isAbsolute(symbol, value, ordinal))
    {
        ServiceClass::writeShortData(currSection, value, outputData);
    }
    else if (symbolTable.isLocal(symbol, value, section))
    {
        outputData.insertRelocData(currSection, "R_386_16", section, "data");
        ServiceClass::writeShortData(currSection, value, outputData);
    }
    else if (symbolTable.isGlobal(symbol, value, ordinal, section))
    {
        outputData.insertRelocData(currSection, "R_386_16", ordinal, "data");
        ServiceClass::writeShortData(currSection, 0, outputData);
    }
}

void ServiceClass::writeSymbolRelAddrInstr(SymbolTable& symbolTable, int currSection, string symbol, OutputData& outputData)
{
    int value = 0;
    int section = 0;
    int ordinal = 0;

    if (symbolTable.isAbsolute(symbol, value, ordinal))
    {
        outputData.insertRelocData(currSection, "R_386_PC16_ABS", ordinal, "instr");
        ServiceClass::writeShortInstr(currSection, value - 2, outputData);
    }
    else if (symbolTable.isLocal(symbol, value, section))
    {
        if (section != currSection)
        {
            outputData.insertRelocData(currSection, "R_386_PC16", section, "instr");
            ServiceClass::writeShortInstr(currSection, value - 2, outputData);
        }
        else
        {
            int lc = 0;
            outputData.getLocalCounter(currSection, lc);
            ServiceClass::writeShortInstr(currSection, value - 2 - lc, outputData);
        }
    }
    else if (symbolTable.isGlobal(symbol, value, ordinal, section))
    {
        if (section != currSection)
        {
            outputData.insertRelocData(currSection, "R_386_PC16", ordinal, "instr");
            ServiceClass::writeShortInstr(currSection, 0 - 2, outputData);
        }
        else
        {
            int lc = 0;
            outputData.getLocalCounter(currSection, lc);
            ServiceClass::writeShortInstr(currSection, value - 2 - lc, outputData);
        }
       
    }
}

void ServiceClass::writeDataInstructions(SymbolTable& symbolTable, int currSection, short int instruction,  bool& condition, int& lineNumber,
    string regName1, string operandName, OutputData& outputData)
{
    short int addrMode = 0;

    ServiceClass::writeByte(currSection, instruction, outputData);

    if (regex_match(operandName, RegularExpressions::REG_DIR))
    {
        addrMode = 0x01;
        string regName2 = operandName;

        ServiceClass::writeTwoRegisters(currSection, regName1, regName2, outputData);
        ServiceClass::writeByte(currSection, addrMode, outputData);
    }
    else if (regex_match(operandName, RegularExpressions::REG_IND))
    {
        addrMode = 0x02;
        string regName2 = operandName.substr(1, operandName.find("]") - 1);
        regName2 = Assembler::trimString(regName2);

        ServiceClass::writeTwoRegisters(currSection, regName1, regName2, outputData);
        ServiceClass::writeByte(currSection, addrMode, outputData);
    }
    else if (regex_match(operandName, RegularExpressions::REG_IND_LITERAL))
    {
        addrMode = 0x03;
        operandName = operandName.substr(1, operandName.find("]") - 1);
        operandName = Assembler::trimString(operandName);
        string regName2 = operandName.substr(0, operandName.find("+"));
        regName2 = Assembler::trimString(regName2);
        operandName.erase(0, operandName.find("+") + 1);

        string immVal = Assembler::trimString(operandName);
        short int value = 0;

        if (regex_match(immVal, RegularExpressions::DEC_NUM))
        {
            value = stoi(immVal);
        }
        else if (regex_match(immVal, RegularExpressions::HEX_NUM))
        {
            value = stoi(immVal, nullptr, 16);
        }

        ServiceClass::writeTwoRegisters(currSection, regName1, regName2, outputData);
        ServiceClass::writeByte(currSection, addrMode, outputData);
        ServiceClass::writeShortInstr(currSection, value, outputData);
    }
    else if (regex_match(operandName, RegularExpressions::REG_IND_SYMBOL))
    {
        addrMode = 0x03;
        operandName = operandName.substr(1, operandName.find("]") - 1);
        operandName = Assembler::trimString(operandName);
        string regName2 = operandName.substr(0, operandName.find("+"));
        regName2 = Assembler::trimString(regName2);
        operandName.erase(0, operandName.find("+") + 1);
        string symbol = Assembler::trimString(operandName);

        if (!symbolTable.checkDefinedExternGlobal(symbol))
        {
            cout << "Error!!! on line " << lineNumber << endl;
            condition = false;
        }
        else
        {
            ServiceClass::writeTwoRegisters(currSection, regName1, regName2, outputData);
            ServiceClass::writeByte(currSection, addrMode, outputData);
            ServiceClass::writeSymbolAbsAddrInstr(symbolTable, currSection, symbol, outputData);
        }
    }
    else if (regex_match(operandName, RegularExpressions::IMM_LITERAL))
    {
        addrMode = 0x00;

        string immVal = operandName.substr(1);
        short int value = 0;

        if (regex_match(immVal, RegularExpressions::DEC_NUM))
        {
            value = stoi(immVal);
        }
        else if (regex_match(immVal, RegularExpressions::HEX_NUM))
        {
            value = stoi(immVal, nullptr, 16);
        }

        ServiceClass::writeRegister(currSection, regName1, outputData);
        ServiceClass::writeByte(currSection, addrMode, outputData);
        ServiceClass::writeShortInstr(currSection, value, outputData);
    }
    else if (regex_match(operandName, RegularExpressions::IMM_SYMBOL))
    {
        addrMode = 0x00;
        string symbol = operandName.substr(1);
        if (!symbolTable.checkDefinedExternGlobal(symbol))
        {
            cout << "Error!!! on line " << lineNumber << endl;
            condition = false;
        }
        else
        {
            ServiceClass::writeRegister(currSection, regName1, outputData);
            ServiceClass::writeByte(currSection, addrMode, outputData);
            ServiceClass::writeSymbolAbsAddrInstr(symbolTable, currSection, symbol, outputData);
        }
    }
    else if (regex_match(operandName, RegularExpressions::MEM_LITERAL))
    {
        addrMode = 0x04;

        string immVal = operandName;
        short int value = 0;

        if (regex_match(immVal, RegularExpressions::DEC_NUM))
        {
            value = stoi(immVal);
        }
        else if (regex_match(immVal, RegularExpressions::HEX_NUM))
        {
            value = stoi(immVal, nullptr, 16);
        }

        ServiceClass::writeRegister(currSection, regName1, outputData);
        ServiceClass::writeByte(currSection, addrMode, outputData);
        ServiceClass::writeShortInstr(currSection, value, outputData);
    }
    else if (regex_match(operandName, RegularExpressions::MEM_SYMBOL))
    {
        addrMode = 0x04;
        string symbol = operandName;
        if (!symbolTable.checkDefinedExternGlobal(symbol))
        {
            cout << "Error!!! on line " << lineNumber << endl;
            condition = false;
        }
        else
        {
            ServiceClass::writeRegister(currSection, regName1, outputData);
            ServiceClass::writeByte(currSection, addrMode, outputData);
            ServiceClass::writeSymbolAbsAddrInstr(symbolTable, currSection, symbol, outputData);
        }
    }
    else if (regex_match(operandName, RegularExpressions::PC_MEM_SYMBOL))
    {
        addrMode = 0x03;
        string symbol = operandName.substr(1);
        string regName2 = "pc";

        if (!symbolTable.checkDefinedExternGlobal(symbol))
        {
            cout << "Error!!! on line " << lineNumber << endl;
            condition = false;
        }
        else
        {
            ServiceClass::writeTwoRegisters(currSection, regName1, regName2, outputData);
            ServiceClass::writeByte(currSection, addrMode, outputData);
            ServiceClass::writeSymbolRelAddrInstr(symbolTable, currSection, symbol, outputData);
        }
    }
}

void ServiceClass::writePushPop(int currSection, short int instruction, short int addrMode, string regName1, OutputData& outputData)
{
    string regName2 = "sp";
    ServiceClass::writeByte(currSection, instruction, outputData);
    ServiceClass::writeTwoRegisters(currSection, regName1, regName2, outputData);
    ServiceClass::writeByte(currSection, addrMode, outputData);
}

void ServiceClass::writeJumpInstructions(SymbolTable& symbolTable, int currSection, short int instruction, bool& condition, int& lineNumber, string operandName, OutputData& outputData)
{
    short int addrMode = 0;

    ServiceClass::writeByte(currSection, instruction, outputData);

    if (regex_match(operandName, RegularExpressions::REG_DIR_JMP))
    {
        addrMode = 0x01;
        string regName = operandName.substr(1);

        ServiceClass::writeRegisterJmp(currSection, regName, outputData);
        ServiceClass::writeByte(currSection, addrMode, outputData);
    }
    else if (regex_match(operandName, RegularExpressions::REG_IND_JMP))
    {
        addrMode = 0x02;
        operandName = operandName.substr(1);
        string regName = operandName.substr(1, operandName.find("]") - 1);
        regName = Assembler::trimString(regName);

        ServiceClass::writeRegisterJmp(currSection, regName, outputData);
        ServiceClass::writeByte(currSection, addrMode, outputData);
    }
    else if (regex_match(operandName, RegularExpressions::REG_IND_LITERAL_JMP))
    {
        addrMode = 0x03;
        operandName = operandName.substr(1);
        operandName = operandName.substr(1, operandName.find("]") - 1);
        operandName = Assembler::trimString(operandName);
        string regName = operandName.substr(0, operandName.find("+"));
        regName = Assembler::trimString(regName);
        operandName.erase(0, operandName.find("+") + 1);

        string immVal = Assembler::trimString(operandName);
        short int value = 0;

        if (regex_match(immVal, RegularExpressions::DEC_NUM))
        {
            value = stoi(immVal);
        }
        else if (regex_match(immVal, RegularExpressions::HEX_NUM))
        {
            value = stoi(immVal, nullptr, 16);
        }

        ServiceClass::writeRegisterJmp(currSection, regName, outputData);
        ServiceClass::writeByte(currSection, addrMode, outputData);
        ServiceClass::writeShortInstr(currSection, value, outputData);
    }
    else if (regex_match(operandName, RegularExpressions::REG_IND_SYMBOL_JMP))
    {
        addrMode = 0x03;
        operandName = operandName.substr(1);
        operandName = operandName.substr(1, operandName.find("]") - 1);
        operandName = Assembler::trimString(operandName);
        string regName = operandName.substr(0, operandName.find("+"));
        regName = Assembler::trimString(regName);
        operandName.erase(0, operandName.find("+") + 1);
        string symbol = Assembler::trimString(operandName);

        if (!symbolTable.checkDefinedExternGlobal(symbol))
        {
            cout << "Error!!! on line " << lineNumber << endl;
            condition = false;
        }
        else
        {
            ServiceClass::writeRegisterJmp(currSection, regName, outputData);
            ServiceClass::writeByte(currSection, addrMode, outputData);
            ServiceClass::writeSymbolAbsAddrInstr(symbolTable, currSection, symbol, outputData);
        }
    }
    else if (regex_match(operandName, RegularExpressions::IMM_LITERAL_JMP))
    {
        addrMode = 0x00;

        string immVal = operandName;
        short int value = 0;

        if (regex_match(immVal, RegularExpressions::DEC_NUM))
        {
            value = stoi(immVal);
        }
        else if (regex_match(immVal, RegularExpressions::HEX_NUM))
        {
            value = stoi(immVal, nullptr, 16);
        }

        ServiceClass::writeNoRegisterJmp(currSection, outputData);
        ServiceClass::writeByte(currSection, addrMode, outputData);
        ServiceClass::writeShortInstr(currSection, value, outputData);
    }
    else if (regex_match(operandName, RegularExpressions::IMM_SYMBOL_JMP))
    {
        addrMode = 0x00;
        string symbol = operandName;

        if (!symbolTable.checkDefinedExternGlobal(symbol))
        {
            cout << "Error!!! on line " << lineNumber << endl;
            condition = false;
        }
        else
        {
            ServiceClass::writeNoRegisterJmp(currSection, outputData);
            ServiceClass::writeByte(currSection, addrMode, outputData);
            ServiceClass::writeSymbolAbsAddrInstr(symbolTable, currSection, symbol, outputData);
        }
    }
    else if (regex_match(operandName, RegularExpressions::PC_IMM_SYMBOL_JMP))
    {
        addrMode = 0x05;
        string symbol = operandName.substr(1);
        string regName = "pc";

        if (!symbolTable.checkDefinedExternGlobal(symbol))
        {
            cout << "Error!!! on line " << lineNumber << endl;
            condition = false;
        }
        else
        {
            ServiceClass::writeRegisterJmp(currSection, regName, outputData);
            ServiceClass::writeByte(currSection, addrMode, outputData);
            ServiceClass::writeSymbolRelAddrInstr(symbolTable, currSection, symbol, outputData);
        }
    }
    else if (regex_match(operandName, RegularExpressions::MEM_LITERAL_JMP))
    {
        addrMode = 0x04;

        string immVal = operandName.substr(1);
        short int value = stoi(immVal);

        if (regex_match(immVal, RegularExpressions::DEC_NUM))
        {
            value = stoi(immVal);
        }
        else if (regex_match(immVal, RegularExpressions::HEX_NUM))
        {
            value = stoi(immVal, nullptr, 16);
        }

        ServiceClass::writeNoRegisterJmp(currSection, outputData);
        ServiceClass::writeByte(currSection, addrMode, outputData);
        ServiceClass::writeShortInstr(currSection, value, outputData);
    }
    else if (regex_match(operandName, RegularExpressions::MEM_SYMBOL_JMP))
    {
        addrMode = 0x04;
        string symbol = operandName.substr(1);

        if (!symbolTable.checkDefinedExternGlobal(symbol))
        {
            cout << "Error!!! on line " << lineNumber << endl;
            condition = false;
        }
        else
        {
            ServiceClass::writeNoRegisterJmp(currSection, outputData);
            ServiceClass::writeByte(currSection, addrMode, outputData);
            ServiceClass::writeSymbolAbsAddrInstr(symbolTable, currSection, symbol, outputData);
        }
    }
}

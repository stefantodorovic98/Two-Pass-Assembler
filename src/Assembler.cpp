#include "Assembler.h"

string Assembler::trimString(string arg)
{
    if (arg.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
    {
        arg.erase(0, arg.find_first_not_of(" \t"));
        arg.erase(arg.find_last_not_of(" \t") + 1);
    }
    return arg;
}

void Assembler::parseLineWithOneReg(vector<string>& oneLine, string operation, string line)
{
    string delimeter = " ";
    line = line.substr(line.find(delimeter));
    string value = trimString(line);

    oneLine.push_back(operation);
    oneLine.push_back(value);
}

void Assembler::parseLineWithTwoRegs(vector<string>& oneLine, string operation, string line)
{
    string delimeter = " ";
    line = line.substr(line.find(delimeter));
    string values = trimString(line);
    string symbol1, symbol2;

    delimeter = ",";
    symbol1 = values.substr(0, values.find(delimeter));
    symbol1 = trimString(symbol1);

    values.erase(0, values.find(delimeter) + 1);
    symbol2 = trimString(values);

    oneLine.push_back(operation);
    oneLine.push_back(symbol1);
    oneLine.push_back(symbol2);
}

void Assembler::parseLineWithOperand(vector<string>& oneLine, string operation, string line)
{
    string delimeter = " ";
    line = line.substr(line.find(delimeter));
    string value = trimString(line);

    oneLine.push_back(operation);
    oneLine.push_back(value);
}

void Assembler::parseLineWithRegOperand(vector<string>& oneLine, string operation, string line)
{
    string delimeter = " ";
    line = line.substr(line.find(delimeter));
    string values = trimString(line);
    string symbol1, symbol2;

    delimeter = ",";
    symbol1 = values.substr(0, values.find(delimeter));
    symbol1 = trimString(symbol1);

    values.erase(0, values.find(delimeter) + 1);
    symbol2 = trimString(values);

    oneLine.push_back(operation);
    oneLine.push_back(symbol1);
    oneLine.push_back(symbol2);
}

void Assembler::FirstPass(ifstream& inputFile, ofstream& outputFile, SymbolTable& symbolTable, vector<vector<string>>& parsedCode, bool& condition)
{
    int ordinal = 0;
    int curr_section = 0;
    lineNumber = 0;
	Row und("UND", 0, lc, "local", ordinal++, 0, false, false);
	symbolTable.insertSymbol(und);
    Row abs("ABS", 1, lc, "local", ordinal++, 0, false, false);
    symbolTable.insertSymbol(abs);
    string line;

    while (getline(inputFile, line) && condition)
    {
        line = trimString(line);
        lineNumber++;
        vector<string> oneLine;
        //Cut comment
        if (line.find("#") != string::npos)
        {
            line = line.substr(0, line.find("#"));
            line = trimString(line);
        }
        //Label
        if (regex_match(line, RegularExpressions::LABEL))
        {
            string delimeter = ":";
            string symbol = line.substr(0, line.find(delimeter));
            if (symbolTable.existsLabel(symbol))
            {
                cout << "Error!!!! on line " << lineNumber << endl;
                condition = false;
                break;
            }
            else
            {
                if (!symbolTable.labelExtern(symbol)) {
                    if (!symbolTable.defineLabel(symbol, curr_section, lc))
                    {
                        Row newSymbol(symbol, curr_section, lc, "local", ordinal++, 0, false, false);
                        symbolTable.insertSymbol(newSymbol);
                    }
                }
                else
                {
                    cout << "Error!!!! on line " << lineNumber << endl;
                    condition = false;
                    break;
                }
                
            }
            line.erase(0, line.find(delimeter) + 1);
            line = trimString(line);
        }
        //Section
        if (regex_match(line, RegularExpressions::SECTION))
        {
            string delimeter = " ";
            line = line.substr(line.find(delimeter));
            string section = trimString(line);

            oneLine.push_back(".section");
            oneLine.push_back(section);

            int number = 0;
            int size = 0;
            symbolTable.setSectionSize(curr_section, lc);

            /*if (symbolTable.globalSection(section, ordinal))
            {
                curr_section = ordinal++;
                lc = 0;
            }*/

            if (symbolTable.existsSection(section, number, size))
            {
                curr_section = number;
                lc = size;
            }
            else
            {
                Row newSymbol(section, ordinal, 0, "local", ordinal, 0, false, false);
                curr_section = ordinal++;
                lc = 0;
                symbolTable.insertSymbol(newSymbol);
            }

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Global
        else if (regex_match(line, RegularExpressions::GLOBAL))
        {
            string delimeter = " ";
            line = line.substr(line.find(delimeter));
            string values = trimString(line);
            string symbol;

            delimeter = ",";
            while (values.find(delimeter) != string::npos)
            {
                symbol = values.substr(0, values.find(delimeter));
                symbol = trimString(symbol);

                if (symbolTable.alreadyWasGlobal(symbol))
                {
                    cout << "Error!!!! on line " << lineNumber << endl;
                    condition = false;
                    break;
                }
                else
                {
                    if (!symbolTable.checkForGlobal(symbol))
                    {
                        Row newSymbol(symbol, 0, 0, "global", ordinal, 0, false, true);
                        ordinal++;
                        symbolTable.insertSymbol(newSymbol);
                    }
                }

                values.erase(0, values.find(delimeter) + 1);
            }
            symbol = trimString(values);

            if (symbolTable.alreadyWasGlobal(symbol))
            {
                cout << "Error!!!! on line " << lineNumber << endl;
                condition = false;
                break;
            }
            else
            {
                if (!symbolTable.checkForGlobal(symbol))
                {
                    Row newSymbol(symbol, 0, 0, "global", ordinal, 0, false, true);
                    ordinal++;
                    symbolTable.insertSymbol(newSymbol);
                }
            }
        }
        //Extern
        else if (regex_match(line, RegularExpressions::EXTERN))
        {
            string delimeter = " ";
            line = line.substr(line.find(delimeter));
            string values = trimString(line);
            string symbol;

            delimeter = ",";
            while (values.find(delimeter) != string::npos)
            {
                symbol = values.substr(0, values.find(delimeter));
                symbol = trimString(symbol);

                if (!symbolTable.checkDefinedExternGlobal(symbol))
                {
                    Row newSymbol(symbol, 0, 0, "global", ordinal, 0, true, false);
                    ordinal++;
                    symbolTable.insertSymbol(newSymbol);
                }
                else
                {
                    cout << "Error!!!! on line " << lineNumber << endl;
                    condition = false;
                    break;
                }

                values.erase(0, values.find(delimeter) + 1);
            }
            symbol = trimString(values);
            if (!symbolTable.checkDefinedExternGlobal(symbol))
            {
                Row newSymbol(symbol, 0, 0, "global", ordinal, 0, true, false);
                ordinal++;
                symbolTable.insertSymbol(newSymbol);
            }  
            else
            {
                cout << "Error!!!! on line "<< lineNumber << endl;
                condition = false;
                break;
            }
        }
        //Word
        else if (regex_match(line, RegularExpressions::WORD))
        {
            string delimeter = " ";
            line = line.substr(line.find(delimeter));
            string values = trimString(line);
            string symbol;

            oneLine.push_back(".word");

            delimeter = ",";
            while (values.find(delimeter) != string::npos)
            {
                symbol = values.substr(0, values.find(delimeter));
                symbol = trimString(symbol);

                lc += 2;
                oneLine.push_back(symbol);

                values.erase(0, values.find(delimeter) + 1);
            }
            symbol = trimString(values);

            lc += 2;
            oneLine.push_back(symbol);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Skip
        else if (regex_match(line, RegularExpressions::SKIP))
        {
            string delimeter = " ";
            line = line.substr(line.find(delimeter));
            string values = trimString(line);

            oneLine.push_back(".skip");
            oneLine.push_back(values);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);

            if (regex_match(values, RegularExpressions::DEC_NUM))
            {
                lc += stoi(values);
            }
            else if (regex_match(values, RegularExpressions::HEX_NUM))
            {
                lc += stoi(values, nullptr, 16);
            }
            
        }
        //Equ
        else if (regex_match(line, RegularExpressions::EQU))
        {
            string delimeter = " ";
            line = line.substr(line.find(delimeter));
            string values = trimString(line);
            string symbol;

            delimeter = ",";

            symbol = values.substr(0, values.find(delimeter));
            symbol = trimString(symbol);
            values.erase(0, values.find(delimeter) + 1);

            string stringVal = trimString(values);
            int intVal = 0;

            if (regex_match(stringVal, RegularExpressions::DEC_NUM))
            {
                intVal = stoi(stringVal);
            }
            else if (regex_match(stringVal, RegularExpressions::HEX_NUM))
            {
                intVal = stoi(stringVal, nullptr, 16);
            }

            if (symbolTable.existsLabel(symbol))
            {
                cout << "Error!!!! on line " << lineNumber << endl;
                condition = false;
                break;
            }
            else
            {
                if (!symbolTable.labelExtern(symbol)) {
                    if (!symbolTable.defineLabel(symbol, 1, intVal))
                    {
                        Row newSymbol(symbol, 1, intVal, "local", ordinal++, 0, false, false);
                        symbolTable.insertSymbol(newSymbol);
                    }
                }
                else
                {
                    cout << "Error!!!! on line " << lineNumber << endl;
                    condition = false;
                    break;
                }
            }

        }
        //End
        else if (regex_match(line, RegularExpressions::END))
        {
            symbolTable.setSectionSize(curr_section, lc);
            oneLine.push_back(".end");
            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
            break;
        }
        //Halt
        else if (regex_match(line, RegularExpressions::HALT))
        {
            lc++;
            oneLine.push_back("halt");

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Int
        else if (regex_match(line, RegularExpressions::INT))
        {
            lc += 2;
            parseLineWithOneReg(oneLine, "int", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Iret
        else if (regex_match(line, RegularExpressions::IRET))
        {
            lc++;
            oneLine.push_back("iret");

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Ret
        else if (regex_match(line, RegularExpressions::RET))
        {
            lc++;
            oneLine.push_back("ret");

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }        
        //Push
        else if (regex_match(line, RegularExpressions::PUSH))
        {
            lc += 3;
            parseLineWithOneReg(oneLine, "push", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Pop
        else if (regex_match(line, RegularExpressions::POP))
        {
            lc += 3;
            parseLineWithOneReg(oneLine, "pop", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Xchg
        else if (regex_match(line, RegularExpressions::XCHG))
        {
            lc += 2;
            parseLineWithTwoRegs(oneLine, "xchg", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Add
        else if (regex_match(line, RegularExpressions::ADD))
        {
            lc += 2;
            parseLineWithTwoRegs(oneLine, "add", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Sub
        else if (regex_match(line, RegularExpressions::SUB))
        {
            lc += 2;
            parseLineWithTwoRegs(oneLine, "sub", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Mul
        else if (regex_match(line, RegularExpressions::MUL))
        {
            lc += 2;
            parseLineWithTwoRegs(oneLine, "mul", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Div
        else if (regex_match(line, RegularExpressions::DIV))
        {
            lc += 2;
            parseLineWithTwoRegs(oneLine, "div", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Cmp
        else if (regex_match(line, RegularExpressions::CMP))
        {
            lc += 2;
            parseLineWithTwoRegs(oneLine, "cmp", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Not
        else if (regex_match(line, RegularExpressions::NOT))
        {
            lc += 2;
            parseLineWithOneReg(oneLine, "not", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //And
        else if (regex_match(line, RegularExpressions::AND))
        {
            lc += 2;
            parseLineWithTwoRegs(oneLine, "and", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Or
        else if (regex_match(line, RegularExpressions::OR))
        {
            lc += 2;
            parseLineWithTwoRegs(oneLine, "or", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Xor
        else if (regex_match(line, RegularExpressions::XOR))
        {
            lc += 2;
            parseLineWithTwoRegs(oneLine, "xor", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Test
        else if (regex_match(line, RegularExpressions::TEST))
        {
            lc += 2;
            parseLineWithTwoRegs(oneLine, "test", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Shl
        else if (regex_match(line, RegularExpressions::SHL))
        {
            lc += 2;
            parseLineWithTwoRegs(oneLine, "shl", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Shr
        else if (regex_match(line, RegularExpressions::SHR))
        {
            lc += 2;
            parseLineWithTwoRegs(oneLine, "shr", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Ldr_3_B
        else if (regex_match(line, RegularExpressions::LDR_3_B))
        {
            lc += 3;
            parseLineWithRegOperand(oneLine, "ldr", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Ldr_5_B
        else if (regex_match(line, RegularExpressions::LDR_5_B))
        {
            lc += 5;
            parseLineWithRegOperand(oneLine, "ldr", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Str_3_B
        else if (regex_match(line, RegularExpressions::STR_3_B))
        {
            lc += 3;
            parseLineWithRegOperand(oneLine, "str", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Str_5_B
        else if (regex_match(line, RegularExpressions::STR_5_B))
        {
            lc += 5;
            parseLineWithRegOperand(oneLine, "str", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Call_3_B
        else if (regex_match(line, RegularExpressions::CALL_3_B))
        {
            lc += 3;
            parseLineWithOperand(oneLine, "call", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Call_5_B
        else if (regex_match(line, RegularExpressions::CALL_5_B))
        {
            lc += 5;
            parseLineWithOperand(oneLine, "call", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Jmp_3_B
        else if (regex_match(line, RegularExpressions::JMP_3_B))
        {
            lc += 3;
            parseLineWithOperand(oneLine, "jmp", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Jmp_5_B
        else if (regex_match(line, RegularExpressions::JMP_5_B))
        {
            lc += 5;
            parseLineWithOperand(oneLine, "jmp", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Jeq_3_B
        else if (regex_match(line, RegularExpressions::JEQ_3_B))
        {
            lc += 3;
            parseLineWithOperand(oneLine, "jeq", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Jeq_5_B
        else if (regex_match(line, RegularExpressions::JEQ_5_B))
        {
            lc += 5;
            parseLineWithOperand(oneLine, "jeq", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Jne_3_B
        else if (regex_match(line, RegularExpressions::JNE_3_B))
        {
            lc += 3;
            parseLineWithOperand(oneLine, "jne", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Jne_5_B
        else if (regex_match(line, RegularExpressions::JNE_5_B))
        {
            lc += 5;
            parseLineWithOperand(oneLine, "jne", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Jgt_3_B
        else if (regex_match(line, RegularExpressions::JGT_3_B))
        {
            lc += 3;
            parseLineWithOperand(oneLine, "jgt", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Jgt_5_B
        else if (regex_match(line, RegularExpressions::JGT_5_B))
        {
            lc += 5;
            parseLineWithOperand(oneLine, "jgt", line);

            if (oneLine.size() > 0)
                parsedCode.push_back(oneLine);
        }
        //Parse error!!!
        else if(!regex_match(line, RegularExpressions::BLANK) && !regex_match(line, RegularExpressions::COMMENT))
        {
            cout << "Error!!!! on line " << lineNumber << "! Can't parse a line !!!" << endl;
            condition = false;
            break;
        }
    }
    //symbolTable.print();
    symbolTable.rearrangeData();
    symbolTable.print();
    symbolTable.print(outputFile);
    if (symbolTable.undefinedGlobal() == true) condition = false;
}

// SecondPass

void Assembler::SecondPass(ofstream& outputFile, SymbolTable& symbolTable, vector<vector<string>>& parsedCode,
    OutputData& outputData, bool& condition)
{
    int currSection = 0;
    lineNumber = 0;
    for (unsigned int i = 0; i < parsedCode.size() && condition; i++)
    {
        lineNumber++;
        vector<string> line = parsedCode[i];
     
        if (line[0] == ".section")
        {       
            string name = line[1];
            if (!outputData.sectionExists(name, currSection))
            {
                int ordinal = symbolTable.getOrdinal(name);
                OutputSection newSection(name, ordinal);
                outputData.insertSection(newSection);
                currSection = ordinal;
            } 
        }
        else if (line[0] == ".word")
        {
            for (unsigned int j = 1; j < line.size() && condition; j++)
            {
                if (!(regex_match(line[j], RegularExpressions::DEC_NUM) || regex_match(line[j], RegularExpressions::HEX_NUM)))
                {
                    string symbol = line[j];
                    if (!symbolTable.checkDefinedExternGlobal(symbol))
                    {
                        cout << "Error!!! on line " << lineNumber << endl;
                        condition = false;
                        break;
                    }
                    else
                    {
                        ServiceClass::writeSymbolAbsAddrData(symbolTable, currSection, symbol, outputData);
                    }
                }
                else if (regex_match(line[j], RegularExpressions::DEC_NUM))
                {
                    short int num = stoi(line[j]);
                    ServiceClass::writeShortData(currSection, num, outputData);
                }
                else if (regex_match(line[j], RegularExpressions::HEX_NUM))
                {
                    short int num = stoi(line[j], nullptr, 16);
                    ServiceClass::writeShortData(currSection, num, outputData);
                }
            }
        }
        else if (line[0] == ".skip")
        {
            short int num = 0;
            if (regex_match(line[1], RegularExpressions::DEC_NUM))
            {
                num = stoi(line[1]);
            }
            else if (regex_match(line[1], RegularExpressions::HEX_NUM))
            {
                num = stoi(line[1], nullptr, 16);
            }
            
            ServiceClass::writeSkipBytes(currSection, num, outputData);
        }
        else if (line[0] == ".end")
        {
            break;
        }
        else if (line[0] == "halt")
        {
            short int instruction = 0x00;
            ServiceClass::writeByte(currSection, instruction, outputData);
        }
        else if (line[0] == "int")
        {
            short int instruction = 0x10;
            string regName = line[1];
            ServiceClass::writeInstructionWithOneReg(currSection, instruction, regName, outputData);
        }
        else if (line[0] == "iret")
        {
            short int instruction = 0x20;
            ServiceClass::writeByte(currSection, instruction, outputData);
        }
        else if (line[0] == "ret")
        {
            short int instruction = 0x40;
            ServiceClass::writeByte(currSection, instruction, outputData);
        }
        //Popravi
        else if (line[0] == "push")
        {
            short int instruction = 0xB0;
            short int addrMode = 0x12;
            string regName1 = line[1];

            ServiceClass::writePushPop(currSection, instruction, addrMode, regName1, outputData);
        }
        //Popravi
        else if (line[0] == "pop")
        {
            short int instruction = 0xA0;
            short int addrMode = 0x42;
            string regName1 = line[1];

            ServiceClass::writePushPop(currSection, instruction, addrMode, regName1, outputData);
        }
        else if (line[0] == "xchg")
        {
            short int instruction = 0x60;
            string regName1 = line[1];
            string regName2 = line[2];
            ServiceClass::writeInstuctionWithTwoRegs(currSection, instruction, regName1, regName2, outputData);
        }
        else if (line[0] == "add")
        {
            short int instruction = 0x70;
            string regName1 = line[1];
            string regName2 = line[2];
            ServiceClass::writeInstuctionWithTwoRegs(currSection, instruction, regName1, regName2, outputData);
        }
        else if (line[0] == "sub")
        {
            short int instruction = 0x71;
            string regName1 = line[1];
            string regName2 = line[2];
            ServiceClass::writeInstuctionWithTwoRegs(currSection, instruction, regName1, regName2, outputData);
        }
        else if (line[0] == "mul")
        {
            short int instruction = 0x72;
            string regName1 = line[1];
            string regName2 = line[2];
            ServiceClass::writeInstuctionWithTwoRegs(currSection, instruction, regName1, regName2, outputData);
        }
        else if (line[0] == "div")
        {
            short int instruction = 0x73;
            string regName1 = line[1];
            string regName2 = line[2];
            ServiceClass::writeInstuctionWithTwoRegs(currSection, instruction, regName1, regName2, outputData);
        }
        else if (line[0] == "cmp")
        {
            short int instruction = 0x74;
            string regName1 = line[1];
            string regName2 = line[2];
            ServiceClass::writeInstuctionWithTwoRegs(currSection, instruction, regName1, regName2, outputData);
        }
        else if (line[0] == "not")
        {
            short int instruction = 0x80;
            string regName = line[1];
            ServiceClass::writeInstructionWithOneReg(currSection, instruction, regName, outputData);
        }
        else if (line[0] == "and")
        {
            short int instruction = 0x81;
            string regName1 = line[1];
            string regName2 = line[2];
            ServiceClass::writeInstuctionWithTwoRegs(currSection, instruction, regName1, regName2, outputData);
        }
        else if (line[0] == "or")
        {
            short int instruction = 0x82;
            string regName1 = line[1];
            string regName2 = line[2];
            ServiceClass::writeInstuctionWithTwoRegs(currSection, instruction, regName1, regName2, outputData);
        }
        else if (line[0] == "xor")
        {
            short int instruction = 0x83;
            string regName1 = line[1];
            string regName2 = line[2];
            ServiceClass::writeInstuctionWithTwoRegs(currSection, instruction, regName1, regName2, outputData);
        }
        else if (line[0] == "test")
        {
            short int instruction = 0x84;
            string regName1 = line[1];
            string regName2 = line[2];
            ServiceClass::writeInstuctionWithTwoRegs(currSection, instruction, regName1, regName2, outputData);
        }
        else if (line[0] == "shl")
        {
            short int instruction = 0x90;
            string regName1 = line[1];
            string regName2 = line[2];
            ServiceClass::writeInstuctionWithTwoRegs(currSection, instruction, regName1, regName2, outputData);
        }
        else if (line[0] == "shr")
        {
            short int instruction = 0x91;
            string regName1 = line[1];
            string regName2 = line[2];
            ServiceClass::writeInstuctionWithTwoRegs(currSection, instruction, regName1, regName2, outputData);
        }
        else if (line[0] == "ldr")
        {
            string regName1 = line[1];
            string operandName = line[2];

            short int instruction = 0xA0;

            ServiceClass::writeDataInstructions(symbolTable, currSection, instruction, condition, lineNumber,
                regName1, operandName, outputData);
            if (condition == false)
            {
                break;
            } 
        }
        else if (line[0] == "str")
        {
            string regName1 = line[1];
            string operandName = line[2];

            short int instruction = 0xB0;

            ServiceClass::writeDataInstructions(symbolTable, currSection, instruction, condition, lineNumber, regName1, operandName, outputData);
            if (condition == false)
            {
                break;
            }
        }
        else if (line[0] == "call")
        {
            string operandName = line[1];

            short int instruction = 0x30;

            ServiceClass::writeJumpInstructions(symbolTable, currSection, instruction, condition, lineNumber, operandName, outputData);
            if (condition == false)
            {
                break;
            }
        }
        else if (line[0] == "jmp")
        {
            string operandName = line[1];

            short int instruction = 0x50;

            ServiceClass::writeJumpInstructions(symbolTable, currSection, instruction, condition, lineNumber, operandName, outputData);
            if (condition == false)
            {
                break;
            }
        }
        else if (line[0] == "jeq")
        {
            string operandName = line[1];

            short int instruction = 0x51;

            ServiceClass::writeJumpInstructions(symbolTable, currSection, instruction, condition, lineNumber, operandName, outputData);
            if (condition == false)
            {
                break;
            }
        }
        else if (line[0] == "jne")
        {
            string operandName = line[1];

            short int instruction = 0x52;

            ServiceClass::writeJumpInstructions(symbolTable, currSection, instruction, condition, lineNumber, operandName, outputData);
            if (condition == false)
            {
                break;
            }
        }
        else if (line[0] == "jgt")
        {
            string operandName = line[1];

            short int instruction = 0x53;

            ServiceClass::writeJumpInstructions(symbolTable, currSection, instruction, condition, lineNumber, operandName, outputData);
            if (condition == false)
            {
                break;
            }
        }
    }
    outputData.printRelocData();
    outputData.printRelocData(outputFile);
    outputData.printByteData();
    outputData.printByteData(outputFile);
}


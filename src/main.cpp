#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "Assembler.h"
#include "RegularExpressions.h"
#include "SymbolTable.h"
#include "OutputData.h"

#include <iomanip>
#include <sstream>

using namespace std;

int main(int argc, char **argv)
{
    string inputCode = "";
    string outputCode = "";
    if (argc == 4)
    {
        string arg0 = string(argv[0]);
        string arg1 = string(argv[1]);
        string arg2 = string(argv[2]);
        string arg3 = string(argv[3]);
        if (arg1 == "-o")
        {
            outputCode = arg2;
            inputCode = arg3;
        }
        else if (arg2 == "-o")
        {
            inputCode = arg1;
            outputCode = arg3;
        }
        else
        {
            cout << "Set right parameters !!!" << endl;
            return 0;
        }
    }
    else if (argc == 2)
    {
        string arg0 = string(argv[0]);
        string arg1 = string(argv[1]);

        inputCode = arg1;
        outputCode = "tests/izlaz.txt";
    }
    else
    {
        cout << "Set right parameters !!!" << endl;
        return 0;

    }
    
    ifstream inputFile(inputCode);
    ofstream outputFile(outputCode);

    Assembler as;
    SymbolTable symbolTable;
    vector<vector<string>> parsedCode;
    OutputData outputData;
    bool condition = true;

    as.FirstPass(inputFile, outputFile, symbolTable, parsedCode, condition);

    //ofstream outFile("../Linker/myfile.txt", ios::out | ios::binary);
    string output_binary = "../zadatak2/";
    outputCode = outputCode.substr(0, outputCode.find("."));
    outputCode += "_bin.txt";
    output_binary += outputCode;
    
    ofstream outFile(output_binary, ios::out | ios::binary);

    symbolTable.writeBinary(outFile);

    if (!condition) cout << "No second pass! Parse error!";

    if (condition)
    {
        as.SecondPass(outputFile, symbolTable, parsedCode, outputData, condition);
    }
    
    if (condition)
    {
        outputData.writeBinary(outFile);
    }
    

    /*inputFile.close();
    outputFile.close();*/

    return 0;
}


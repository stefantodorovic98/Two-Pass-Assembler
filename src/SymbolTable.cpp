#include "SymbolTable.h"

void SymbolTable::insertSymbol(Row row)
{
	symbolVector.push_back(row);
}

void SymbolTable::print(ofstream& outputFile)
{
	outputFile << "#" << " Symbol Table " << endl;
	outputFile << setw(20) << left << "name" << setw(10) << left << "section";
	outputFile << setw(10) << left << "value" << setw(20) << left << "visibility";
	outputFile << setw(10) << left << "ordinal" << setw(10) << left << "sectionSize" << endl;
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		(*it).print(outputFile);
	}
	outputFile << endl;
}

void SymbolTable::print()
{
	cout << "#" << " Symbol Table " << endl;
	cout << setw(20) << left << "name" << setw(10) << left << "section";
	cout << setw(10) << left << "value" << setw(20) << left << "visibility";
	cout << setw(10) << left << "ordinal" << setw(10) << left << "sectionSize" << endl;
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		(*it).print();
	}
	cout << endl;
}

bool SymbolTable::existsSection(string sectionName, int& ordinal, int& sectionSize)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == sectionName)
		{
			ordinal = (*it).getOrdinal();
			sectionSize = (*it).getSectionSize();
			return true;
		}
	}
	ordinal = -1;
	return false;
}

void SymbolTable::setSectionSize(int ordinal, int sectionSize)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getOrdinal() == ordinal)
		{
			(*it).setSectionSize(sectionSize);
		}
	}
}

bool SymbolTable::existsLabel(string labelName)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == labelName && (*it).getSection() != 0)
		{
			return true;
		}
	}
	return false;
}

bool SymbolTable::alreadyWasGlobal(string labelName)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == labelName && (*it).getVisibility() == "global")
		{
			return true;
		}
	}
	return false;
}

bool SymbolTable::checkForGlobal(string labelName)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == labelName)
		{
			(*it).setVisibility("global");
			return true;
		}
	}
	return false;
}

bool SymbolTable::checkDefinedExternGlobal(string labelName)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == labelName)
		{
			return true;
		}
	}
	return false;
}

bool SymbolTable::defineLabel(string labelName, int section, int value)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == labelName)
		{
			(*it).setSection(section);
			(*it).setValue(value);
			return true;
		}
	}
	return false;
}

bool SymbolTable::labelExtern(string labelName)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == labelName && (*it).getExt() == true)
		{
			return true;
		}
	}
	return false;
}

bool SymbolTable::undefinedGlobal()
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getSection() == 0 && (*it).getGlb() == true)
		{
			return true;
		}
	}
	return false;
}

void SymbolTable::rearrangeData()
{
	vector<Row> temp;
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getSection() == (*it).getOrdinal())
		{
			temp.push_back((*it));
		}
	}
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getSection() != (*it).getOrdinal())
		{
			temp.push_back((*it));
		}
	}
	int ordinal = 0;
	for (vector<Row>::iterator it = temp.begin(); it != temp.end(); ++it)
	{
		if ((*it).getSection() == (*it).getOrdinal())
		{
			for (vector<Row>::iterator itt = temp.begin(); itt != temp.end(); ++itt)
			{
				if ((*itt).getOrdinal() != (*it).getOrdinal() && (*itt).getSection() == (*it).getOrdinal())
				{
					(*itt).setSection(ordinal);
					(*itt).setOrdinal(-10);
				}

			}
			(*it).setSection(ordinal);
		}
		(*it).setOrdinal(ordinal);
		
		ordinal++;
	}
	symbolVector = temp;
}

int SymbolTable::getOrdinal(string name)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == name)
		{
			return (*it).getOrdinal();
		}
	}
	return -100;
}

bool SymbolTable::getRow(string name, Row& row)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == name)
		{
			row = (*it);
			return true;
		}
	}
	
	return false;
}

bool SymbolTable::isLocal(string name, int& value, int& section)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == name && (*it).getVisibility() == "local")
		{
			section = (*it).getSection();
			value = (*it).getValue();
			return true;
		}
	}
	return false;
}

bool SymbolTable::isGlobal(string name, int& value, int& ordinal, int& section)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == name && (*it).getVisibility() == "global")
		{
			ordinal = (*it).getOrdinal();
			value = (*it).getValue();
			section = (*it).getSection();
			return true;
		}
	}
	return false;
}

bool SymbolTable::isAbsolute(string name, int& value, int& ordinal)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == name && (*it).getSection() == 1)
		{
			value = (*it).getValue();
			ordinal = (*it).getOrdinal();
			return true;
		}
	}
	return false;
}

void SymbolTable::writeBinary(ofstream& outputBinary)
{
	unsigned int size = symbolVector.size();
	outputBinary.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		(*it).writeBinary(outputBinary);
	}
}

void SymbolTable::readBinary(ifstream& inputBinary)
{
	unsigned int size = 0;
	inputBinary.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (unsigned int i = 0; i < size; i++)
	{
		Row r;
		r.readBinary(inputBinary);
		symbolVector.push_back(r);
	}
}

bool SymbolTable::globalSection(string sectionName, int section)
{
	for (vector<Row>::iterator it = symbolVector.begin(); it != symbolVector.end(); ++it)
	{
		if ((*it).getName() == sectionName && (*it).getGlb() == true && (*it).getSection() == 0)
		{
			(*it).setSection(section);
			(*it).setOrdinal(section);
			return true;
		}
	}
	return false;
}


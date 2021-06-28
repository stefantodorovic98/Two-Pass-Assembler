#include "OutputData.h"

void OutputData::insertSection(OutputSection data)
{
	dataList.push_back(data);
}

void OutputData::insertRelocData(int currSection, string type, int value, string data_instr)
{
	for (vector<OutputSection>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		if ((*it).getOrdinal() == currSection)
		{
			(*it).insertRelocData(type, value, data_instr);
		}
	}
}

bool OutputData::sectionExists(string name, int& currSection)
{
	for (vector<OutputSection>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		if ((*it).getName() == name)
		{
			currSection = (*it).getOrdinal();
			return true;
		}
	}
	return false;
}

void OutputData::insertByte(int currSection, string byte)
{
	for (vector<OutputSection>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		if ((*it).getOrdinal() == currSection)
		{
			(*it).insertByteData(byte);
		}
	}
}

void OutputData::insertShort(int currSection, string lowByte, string highByte)
{
	for (vector<OutputSection>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		if ((*it).getOrdinal() == currSection)
		{
			
			(*it).insertByteData(lowByte);
			(*it).insertByteData(highByte);
		}
	}
}

void OutputData::insertSkipBytes(int currSection, short int num)
{
	for (vector<OutputSection>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		if ((*it).getOrdinal() == currSection)
		{
			string byte = "00";
			for (int i = 0; i < num; i++)
			{
				(*it).insertByteData(byte);
			}
		}
	}
}

void OutputData::printByteData(ofstream& outputFile)
{
	for (vector<OutputSection>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		(*it).printByteData(outputFile);
		outputFile << endl;
	}
}

void OutputData::printByteData()
{
	for (vector<OutputSection>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		(*it).printByteData();
		cout << endl;
	}
}

void OutputData::printRelocData(ofstream& outputFile)
{
	for (vector<OutputSection>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		(*it).printRelocData(outputFile);
	}
}

void OutputData::printRelocData()
{
	for (vector<OutputSection>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		(*it).printRelocData();
	}
}

bool OutputData::getLocalCounter(int currSection, int& lc)
{
	for (vector<OutputSection>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		if ((*it).getOrdinal() == currSection)
		{
			lc = (*it).getLocalCounter();
			return true;
		}
	}
	return false;
}

void OutputData::writeBinary(ofstream& outputBinary)
{
	unsigned int size = dataList.size();
	outputBinary.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (vector<OutputSection>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		(*it).writeBinary(outputBinary);
	}
}

void OutputData::readBinary(ifstream& inputBinary)
{
	unsigned int size = 0;
	inputBinary.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (unsigned int i = 0; i < size; i++)
	{
		OutputSection o;
		o.readBinary(inputBinary);
		dataList.push_back(o);
	}
}

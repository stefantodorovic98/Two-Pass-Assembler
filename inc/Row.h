#ifndef ROW_H
#define ROW_H

#include <iostream>
#include <string>
#include <fstream>

#include <iomanip>

using namespace std;

class Row
{
private:
	string name;
	int section;
	int value;
	string visibility;
	int ordinal;
	int sectionSize;
	bool ext;
	bool glb;
public:
	Row() :
		name(""), section(0), value(0), visibility(""), ordinal(0), sectionSize(0), ext(false), glb(false)
	{	}
	Row(string name, int section, int value, string visibility, int ordinal, int sectionSize, bool ext, bool glb) : 
		name(name), section(section), value(value), visibility(visibility), ordinal(ordinal), sectionSize(sectionSize), ext(ext), glb(glb)
	{	}

	void print(ofstream& outputFile);
	void print();
	string getName();
	int getOrdinal();
	void setOrdinal(int ordinal);
	void setVisibility(string visibility);
	string getVisibility();
	int getSection();
	void setSection(int section);
	bool getExt();
	bool getGlb();

	int getSectionSize();
	void setSectionSize(int sectionSize);

	void setValue(int value);
	int getValue();

	void writeBinary(ofstream& outputBinary);
	void readBinary(ifstream& inputBinary);
};

#endif


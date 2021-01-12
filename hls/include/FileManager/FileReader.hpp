#pragma once

#include<fstream>
#include<sstream>

#include<FileManager/FileBaseManager.hpp>

using namespace std;

class FileReader : public FileBaseManager
{
	public:
	    //Constructors
		FileReader(string);

		//Actions
		string ReadFileLine(ifstream&,int);
        string ReadFileLine(ifstream&);
        string ReadFile(const int, string pattern, bool);
		string ReadFile();
};

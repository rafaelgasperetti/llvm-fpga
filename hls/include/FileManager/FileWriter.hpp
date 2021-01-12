#pragma once

#include<string.h>
#include<iostream>
#include<fstream>
#include<stdexcept>

#include<FileManager/FileBaseManager.hpp>

using namespace std;

class FileWriter : public FileBaseManager
{
	public:
		//Constructors
		FileWriter(string);

		//Actions
        void WriteFile(string);
};

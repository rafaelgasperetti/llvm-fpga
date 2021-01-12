using namespace std;

#pragma once

class MessageLibrary
{
	public:
		static const int COMPILATION_SUCCESSFULL = 0;
		static const int ERROR_FILE_READING = 1;
		static const int ERROR_FILE_EMPTY = 2;
		static const int ERROR_NOT_SPECIFIED_SOURCE = 3;
		static const int ERROR_CREATING_CX_INDEX = 4;
		static const int ERROR_CREATING_CX_TRANSLATION_UNIT = 5;
		static const int ERROR_VISITING_NODES = 5;
		static const int ERROR_UNEXPECTED_OPERATION = 6;
};

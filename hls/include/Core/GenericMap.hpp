#include <string>

using namespace std;

#pragma once

class GenericMap {
public:
	// Constructor
	GenericMap(string, string, string);

	// Setters and getters
	string GetName();
	void SetName(string);
	string GetType();
	void SetType(string);
	string GetValue();
	void SetValue(string);
	string GetDataWidth();
	void SetDataWidth(string);

private:
	string Name;
	string Type;
	string Value;
	string DataWidth;
};

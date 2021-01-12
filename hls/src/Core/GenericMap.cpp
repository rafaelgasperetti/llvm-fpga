#include<string.h>
#include<iostream>

#include<Core/GenericMap.hpp>

using namespace std;

GenericMap::GenericMap(string name, string type, string dataWidth) {
	SetName(name);
	SetType(type);
	SetDataWidth(dataWidth);
}

string GenericMap::GetName() {
	return Name;
}

void GenericMap::SetName(string name) {
	Name = name;
}

string GenericMap::GetType(){
	return Type;
}

void GenericMap::SetType(string type) {
	Type = type;
}

string GenericMap::GetValue() {
	return Value;
}

void GenericMap::SetValue(string value) {
	Value = value;
}

string GenericMap::GetDataWidth() {
	return Value;
}

void GenericMap::SetDataWidth(string dataWidth) {
	DataWidth = dataWidth;
}

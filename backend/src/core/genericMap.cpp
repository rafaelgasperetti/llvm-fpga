#include <string>
#include "core/genericMap.hpp"

GenericMap::GenericMap(std::string name, std::string type, std::string dataWidth) {
	this->name = name;
	this->type = type;
	this->dataWidth = dataWidth;
}

std::string GenericMap::getName() {
	return name;
}

void GenericMap::setName(std::string name) {
	this->name = name;
}

std::string GenericMap::getType(){
	return type;
}

void GenericMap::setType(std::string type) {
	this->type = type;
}

std::string GenericMap::getValue() {
	return value;
}

void GenericMap::setValue(std::string value) {
	this->value = value;
}

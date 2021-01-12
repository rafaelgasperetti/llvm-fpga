#include <string>
#include "core/port.hpp"
#include "core/signal.hpp"

Port::Port(std::string name, std::string width, std::string type, std::string portDirection) {
	this->name = name;
    this->width = width;
    this->type = type;
    this->portDirection = portDirection;
	this->portSignal = NULL;
	this->setUsedWidthInitial("-1");
	this->setUsedWidthFinal("-1");
	this->setIsWide(false);
}

std::string Port::getName() {
	return name;
}

void Port::setName(std::string name) {
	this->name = name;
}

std::string Port::getWidth() {
	return width;
}

void Port::setWidth(std::string width) {
	this->width = width;
}

void Port::setIsWide(bool isWide) {
	this->isWide = isWide;
}

bool Port::getIsWide() {
	return isWide;
}

std::string Port::getUsedWidthInitial() {
	return usedWidthInitial;
}

void Port::setUsedWidthInitial(std::string usedWidthInitial) {
	this->usedWidthInitial = usedWidthInitial;
}

std::string Port::getUsedWidthFinal() {
	return usedWidthFinal;
}

void Port::setUsedWidthFinal(std::string usedWidthFinal) {
	this->usedWidthFinal = usedWidthFinal;
}

std::string Port::getType() {
	return type;
}

void Port::setType(std::string) {
	this->type = type;
}

std::string Port::getPortDirection() {
	return portDirection;
}

void Port::setPortDirection(std::string portDirection) {
	this->portDirection = portDirection;
}

void Port::setPortSignal(Signal* signal) {
	// TODO: Check portDirection
	//portSignals.push_back(signal);
	this->portSignal = signal;
}

Signal* Port::getPortSignal() {
	return portSignal;
}

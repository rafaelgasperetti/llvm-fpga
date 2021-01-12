#include <string>
#include <vector>
#include <utility>
#include "core/signal.hpp"

using namespace lemon;

Signal::Signal(std::string signalName, std::string type, int dataWidth) : ListDigraph::Arc(){
	this->signalName = signalName;
	this->type = type;
	this->dataWidth = dataWidth;
	//this->receivingPort = NULL;
}

std::string Signal::getSignalName() {
	return signalName;
}

std::string Signal::getType() {
	return type;
}

int Signal::getDataWidth() {
	return dataWidth;
}

void Signal::assignSignals(std::string signal1, std::string signal2, 
	                       std::vector<std::string> &assignedSignals) {
	assignedSignals.push_back(signal1 + " <= " + signal2 + ";\n");
}

/*
void Signal::connectSendingPort(Port* port) {
	sendingPorts.push_back(port);
	port->setPortSignal(this);
}

void Signal::connectReceivingPort(Port* port) {
	if (receivingPort == NULL){
		this->receivingPort = port;
		port->setPortSignal(this);
	}
}*/
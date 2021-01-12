#ifndef SIGNAL_H
#define	SIGNAL_H

#include <string>
#include <vector>
#include <utility>
#include <lemon/list_graph.h>

using namespace lemon;

class Signal : ListDigraph::Arc{
public:
	// Constructor
	Signal(std::string, std::string, int);

	// Setters and getters
	std::string getSignalName();
	std::string getType();
	int getDataWidth();

	static void assignSignals(std::string, std::string, std::vector<std::string> &assignedSignals);

	// Port attribution
	//void connectSendingPort(Port*);
	//void connectReceivingPort(Port*);

private:
	std::string signalName;
	std::string type;
	int dataWidth;
	bool isActive;
	//Port* receivingPort;
	//std::vector<Port*> sendingPorts;
};

#endif	/* SIGNAL_H */

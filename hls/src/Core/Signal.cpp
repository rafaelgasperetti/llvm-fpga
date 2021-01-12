#include<string>
#include<vector>
#include<utility>
#include<iostream>

#include<Core/Signal.hpp>

using namespace std;

Signal::Signal(string signalName, string type, int dataWidth){
	SignalName = signalName;
	Type = type;
	DataWidth = dataWidth;
}

string Signal::GetSignalName() {
	return SignalName;
}

string Signal::GetType() {
	return Type;
}

int Signal::GetDataWidth() {
	return DataWidth;
}

void Signal::SetDataWidth(int dataWidth) {
	DataWidth = dataWidth;
}

void Signal::AssignSignals(string signal1, string signal2, vector<string> assignedSignals) {
	assignedSignals.push_back(signal1 + " <= " + signal2 + ";\n");
}

void Signal::ConnectSendingPort(Port* port) {
	SendingPorts.push_back(port);
}

void Signal::ConnectReceivingPort(Port* port) {
	ReceivingPort = port;
	/*if (ReceivingPort == NULL){
		ReceivingPort = port;
	}*/
}

Port* Signal::GetReceivingPort()
{
    return ReceivingPort;
}

vector<Port*> Signal::GetSendingPorts()
{
    return SendingPorts;
}

Port* Signal::GetSendingPort(string name)
{
    for(Port* p : SendingPorts)
    {
        if(p->GetName().compare(name) == 0)
        {
            return p;
        }
    }
    return NULL;
}

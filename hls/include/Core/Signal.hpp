#ifndef SIGNAL_H
#define	SIGNAL_H

#include<string>
#include<vector>
#include<utility>

#include<Core/Port.hpp>
#include<Core/Component.hpp>

using namespace std;

class Port;
class Component;

class Signal{
public:
	// Constructor
	Signal(string, string, int);

	// Setters and getters
	string GetSignalName();
	string GetType();
	int GetDataWidth();
	void SetDataWidth(int);

	Port* GetReceivingPort();
	vector<Port*> GetSendingPorts();
	Port* GetSendingPort(string);

	static void AssignSignals(string, string, vector<string>);

	// Port attribution
	void ConnectSendingPort(Port*);
	void ConnectReceivingPort(Port*);

private:
	string SignalName;
	string Type;
	int DataWidth;
	bool IsActive;
	Port* ReceivingPort;
	vector<Port*> SendingPorts;
};

#endif	/* SIGNAL_H */

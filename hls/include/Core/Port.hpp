#ifndef PORT_H
#define	PORT_H

#include<string.h>
#include<vector>

#include<Core/Signal.hpp>
#include<Core/Component.hpp>

using namespace std;

class Signal;
class Component;

class Port {
public:
	// Constructor
	Port(string, int, string);
	Port(string, int, string, string);

	// Setters and Getters
	string GetName();
	void SetName(string);
	int GetWidth();
	void SetWidth(int);
	bool IsWide();
	string GetUsedWidthInitial();
	void SetUsedWidthInitial(string);
	string GetUsedWidthFinal();
	void SetUsedWidthFinal(string);
	string GetType();
	void SetType(string);
	string GetPortDirection();
	void SetPortDirection(string);
	void SetPortSignal(Signal*);
	Signal* GetPortSignal();
	Signal* GetPortSignal(string);
	void RemovePortSignal(string);
	vector<Signal*> GetPortSignals();
	void SetParentComponent(Component*);
	Component* GetParentComponent();

private:
	// Attributes of each port
	string Name;
	int Width;
	string UsedWidthInitial;
	string UsedWidthFinal;
	string Type;
	string PortDirection;
	Signal* PortSignal;
	vector<Signal*> PortSignals;
	Component* ParentComponent;
};

#endif

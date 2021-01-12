#include<string.h>
#include<iostream>

#include<Core/Port.hpp>
#include<Core/Signal.hpp>

#include<Tools/StringUtils.hpp>

using namespace std;

Port::Port(string name, int width, string type) {
	SetName(name);
	SetWidth(width);
    SetType(type);
    SetParentComponent(NULL);
}

Port::Port(string name, int width, string type, string portDirection) {
	SetName(name);
	SetWidth(width);
    SetType(type);
    SetPortDirection(portDirection);
	PortSignal = NULL;
	SetParentComponent(NULL);
}

string Port::GetName() {
	return Name;
}

void Port::SetName(string name) {
	Name = name;
}

int Port::GetWidth() {
	return Width;
}

void Port::SetWidth(int width) {
	Width = width;
	SetUsedWidthInitial(to_string(width - 1));
	SetUsedWidthFinal("0");
}

bool Port::IsWide() {
	return GetWidth() > 1 ? true : false;
}

string Port::GetUsedWidthInitial() {
	return UsedWidthInitial;
}

void Port::SetUsedWidthInitial(string usedWidthInitial) {
	UsedWidthInitial = usedWidthInitial;
}

string Port::GetUsedWidthFinal() {
	return UsedWidthFinal;
}

void Port::SetUsedWidthFinal(string usedWidthFinal) {
	UsedWidthFinal = usedWidthFinal;
}

string Port::GetType() {
	return Type;
}

void Port::SetType(string type) {
	Type = type;
}

string Port::GetPortDirection() {
	return PortDirection;
}

void Port::SetPortDirection(string portDirection) {
	PortDirection = portDirection;
}

void Port::SetPortSignal(Signal* signal) {
	// TODO: Check portDirection
	PortSignals.push_back(signal);
    PortSignal = signal;
    if(signal != NULL && signal->GetDataWidth() > GetWidth())
    {
        SetWidth(signal->GetDataWidth());
    }
}

Signal* Port::GetPortSignal() {
	return PortSignal;
}

Signal* Port::GetPortSignal(string name) {
	for(Signal* s : GetPortSignals())
    {
        if(s->GetSignalName().compare(name) == 0)
        {
            return s;
        }
    }
    return NULL;
}

void Port::RemovePortSignal(string name) {
	for(int i = 0; i < GetPortSignals().size(); i++)
    {
        if(GetPortSignals()[i]->GetSignalName().compare(name) == 0)
        {
            GetPortSignals().erase(GetPortSignals().begin() + i);
            return;
        }
    }
}

vector<Signal*> Port::GetPortSignals()
{
    return PortSignals;
}

void Port::SetParentComponent(Component* c)
{
    ParentComponent = c;
}

Component* Port::GetParentComponent()
{
    return ParentComponent;
}

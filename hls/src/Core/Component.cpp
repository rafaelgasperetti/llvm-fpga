#include<string>
#include<iostream>
#include<vector>
#include<unordered_map>

#include<Core/Component.hpp>
#include<Core/Port.hpp>
#include<Core/GenericMap.hpp>
#include<Core/Signal.hpp>
#include<math.h>

using namespace std;

// Component class constructor
Component::Component()
{
    SetDeclared(false);
}

void Component::SetComponentName(string componentName)
{
	ComponentName = componentName;
}

string Component::GetComponentName()
{
	return ComponentName;
}

void Component::SetInstanceName(string instanceName)
{
	InstanceName = instanceName;
}

string Component::GetInstanceName()
{
	return InstanceName;
}

void Component::SetDataWidth(int dataWidth)
{
	DataWidth = dataWidth;
}

int Component::GetDataWidth()
{
	return DataWidth;
}

void Component::SetDeclared(bool declared)
{
    Declared = declared;
}

bool Component::IsDeclared()
{
    return Declared;
}

Port* Component::GetPort(int port)
{
	return Ports[port];
}

Port* Component::GetPort(string name)
{
	return Ports[PortStrings[name]];
}

// Returns the vector of ports
vector<Port*> Component::GetPorts()
{
	return Ports;
}

GenericMap* Component::GetMap(int map)
{
	return Maps[map];
}

GenericMap* Component::GetMap(string map)
{
	return Maps[MapStrings[map]];
}

// Return the vector of maps
vector<GenericMap*> Component::GetMaps()
{
	return Maps;
}

void Component::AddPort(Port* port)
{
	Ports.push_back(port);
}

void Component::AddGenericMap(GenericMap* map)
{
	Maps.push_back(map);
}

void Component::ConnectPortSignal(Port* port, Signal* signal)
{
    port->SetParentComponent(this);
    port->SetPortSignal(signal);
}

void Component::ConnectPortSignal(Port* port, Signal* signal, string selectedPos)
{
    ConnectPortSignal(port, signal);
	port->SetUsedWidthInitial(selectedPos);
}

// Links a port to a signal
void Component::ConnectPortSignal(Port* port, Signal* signal, string initialWidth, string finalWidth)
{
	ConnectPortSignal(port, signal, initialWidth);
	port->SetUsedWidthFinal(finalWidth);
	//port->SetWide(true);

}

void Component::SetGenericMap(GenericMap* map, string value)
{
	map->SetValue(value);
}

bool Component::IsIoComponent() {
	return IoComponent;
}

bool Component::IsBlockRam() {
	return BlockRam;
}

string Component::GetBinaryValue(long long int value, int dataWidth) {
	string binary = "";
    int restValue = 0;
	while(value > 0)
    {
    	restValue = value % 2;
    	value /= 2;
    	binary += to_string(restValue);
	}

	auto a=binary.begin();
    auto b=binary.rbegin();

    while (a<b.base())
    {
    	std::swap(*a++, *b++);
	}

	while(binary.size() < dataWidth)
    {
        binary = "0" + binary;
    }

    return binary;
}

string Component::GetBinaryValue(int value, int dataWidth) {
	string binary = "";
    int restValue = 0;
	while(value > 0)
    {
    	restValue = value % 2;
    	value /= 2;
    	binary += to_string(restValue);
	}

	auto a=binary.begin();
    auto b=binary.rbegin();

    while (a<b.base())
    {
    	std::swap(*a++, *b++);
	}

	while(binary.size() < dataWidth)
    {
        binary = "0" + binary;
    }

    return binary;
}

int Component::GetIntegerValue(string binary)
{
    int i = 0;
    int ret = 0;

    while(i < binary.length() && binary.substr(i,1).compare("0") == 0)
    {
        i++;
    }

    while(i < binary.length())
    {
        if(binary.substr(i,1).compare("1") == 0)
        {
            ret += pow(2,binary.length() - 1 - i);
        }
        i++;
    }

    return ret;
}

long long int Component::GetLongValue(string binary)
{
    int i = 0;
    long long int ret = 0;

    while(i < binary.length() && binary.substr(i,1).compare("0") == 0)
    {
        i++;
    }

    while(i < binary.length())
    {
        if(binary.substr(i,1).compare("1") == 0)
        {
            ret += pow(2,binary.length() - 1 - i);
        }
        i++;
    }

    return ret;
}
/*
void Component::SetClockEnable(int clockEnable)
{
    if(clockEnable >= 0)
    {
        ClockEnable = clockEnable;
    }
    else
    {
        clockEnable = 0;
    }
}

int Component::GetClockEnable()
{
    return ClockEnable;
}

//-1 means it's variable, otherwise means the real delay of the object
void Component::SetClockDelay(int clockDelay)
{
    if(clockDelay >= -1)
    {
        ClockDelay = clockDelay;
    }
    else
    {
        clockDelay = -1;
    }
}

int Component::GetClockDelay()
{
    return ClockDelay;
}
*/
void Component::SetUniversal(bool universal)
{
    Universal = universal;
}

bool Component::IsUniversal()
{
    return Universal;
}

bool Component::HasPort(string name)
{
    for(Port* p : GetPorts())
    {
        if(p->GetName().compare(name) == 0)
        {
            return true;
        }
    }
    return false;
}

void Component::AddDependencyLevel(DependencyLevel* level)
{
    if(level != NULL)
    {
        Levels.push_back(level);
    }
    else
    {
        throw invalid_argument("It is not possible to add a null dependency level to the component.");
    }
}

void Component::RemoveDependencyLevel(int level, int type)
{
    for(int i = 0; i < Levels.size(); i++)
    {
        if(Levels[i]->GetLevel() == level && Levels[i]->GetType() == type)
        {
            Levels.erase(Levels.begin() + i);
            cout << "Removed dependency level " << level << " of type " << type << " from " << GetInstanceName() << endl;
        }
    }
}

void Component::AddDependencyLevels(vector<DependencyLevel*> lvls)
{
    for(DependencyLevel* lvl : lvls)
    {
        AddDependencyLevel(lvl);
    }
}

DependencyLevel* Component::GetDependencyLevel(int level, int type)
{
    for(DependencyLevel* lvl : Levels)
    {
        if(lvl->GetLevel() == level && lvl->GetType() == type)
        {
            return lvl;
        }
    }
    return NULL;
}

DependencyLevel* Component::GetMaxDependencyLevel(int type)
{
    DependencyLevel* ret = NULL;
    for(DependencyLevel* lvl : Levels)
    {
        if((ret == NULL || lvl->GetLevel() > ret->GetLevel()) && type == lvl->GetType())
        {
            ret = lvl;
        }
    }
    return ret;
}

DependencyLevel* Component::GetMaxDependencyLevel()
{
    DependencyLevel* ret = NULL;
    for(DependencyLevel* lvl : Levels)
    {
        if(ret == NULL || lvl->GetLevel() > ret->GetLevel())
        {
            ret = lvl;
        }
    }
    return ret;
}

vector<DependencyLevel*> Component::GetDependencyLevels()
{
    return Levels;
}

void Component::AddDependent(Component* comp)
{
    if(comp != NULL && GetDependent(comp->GetInstanceName()) == NULL)
    {
        Dependents.push_back(comp);
    }
    else
    {
        throw invalid_argument("It is not possible to add a null or repeated component dependent.");
    }
}

Component* Component::GetDependent(string name)
{
    for(Component* c : Dependents)
    {
        if(c-GetInstanceName().compare(name) == 0)
        {
            return c;
        }
    }
    return NULL;
}

void Component::SetParent(Component* comp)
{
    Parent = comp;
}

Component* Component::GetParent()
{
    return Parent;
}

void Component::SetId(int id)
{
    Id = id;
}

int Component::GetId()
{
    return Id;
}

void Component::SetContext(Context* context)
{
    ParentContext = context;
}

Context* Component::GetContext()
{
    return ParentContext;
}

void Component::ConnectDefaultSignals(Design* design)
{
    if(HasPort("clk") && GetPort("clk")->GetPortSignal() == NULL)
    {
        design->ConnectPortSignal(this, GetPort("clk"), design->GetSignal("\\clk\\"), false);
    }
    if(HasPort("reset") && GetPort("reset")->GetPortSignal() == NULL)
    {
        design->ConnectPortSignal(this, GetPort("reset"), design->GetSignal("reset"), false);//should be \\reset\\, but it worked only without it
    }
    if(HasPort("clear") && GetPort("clear")->GetPortSignal() == NULL)
    {
        design->ConnectPortSignal(this, GetPort("clear"), design->GetSignal("\\clear\\"), false);
    }
}

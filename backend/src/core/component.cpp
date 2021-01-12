#include <string>
#include <vector>
#include <unordered_map>

#include "core/component.hpp"
#include "core/port.hpp"
#include "core/genericMap.hpp"
#include "core/signal.hpp"

// Component class constructor
Component::Component() : ListDigraph::Node()
{
	
}

/*
*	-------------------------------------------------------------
*	Setters and getters
*	------------------------------------------------------------- 
*/ 
void Component::setComponentName(std::string name)
{
	componentName = name;
}

std::string Component::getComponentName() 
{
	return componentName;
}

void Component::setInstanceName(std::string name)
{
	instanceName = name;
}

std::string Component::getInstanceName() 
{
	return instanceName;
}

void Component::setDataWidth(int width) 
{
	dataWidth = width;
}
	
int Component::getDataWidth() 
{
	return dataWidth;
}

Port* Component::getPort(int i) 
{
	return ports[i];
}

Port* Component::getPort(std::string name) 
{
	return ports[portStrings[name]];
}

// Returns the vector of ports
std::vector<Port*> Component::getPorts() 
{
	return ports;
}

GenericMap* Component::getMap(int i) 
{
	return maps[i];
}

GenericMap* Component::getMap(std::string name) 
{
	return maps[mapStrings[name]];
}

// Return the vector of maps
std::vector<GenericMap*> Component::getMaps() 
{
	return maps;
}

void Component::addPort(Port* port)
{
	ports.push_back(port);
}

void Component::addGenericMap(GenericMap* map)
{
	maps.push_back(map);
}

void Component::connectPortSignal(Port* port, Signal* signal) 
{
	port->setPortSignal(signal);
}

// Links a port to a signal
void Component::connectPortSignal(Port* port, 
		                          Signal* signal, 
		                          std::string initialWidth, 
		                          std::string finalWidth) 
{
	port->setPortSignal(signal);
	port->setUsedWidthInitial(initialWidth);
	port->setUsedWidthFinal(finalWidth);
	port->setIsWide(true);

}

void Component::connectPortSignal(Port* port, 
		                          Signal* signal, 
		                          std::string selectedPos) 
{
	port->setPortSignal(signal);
	port->setUsedWidthInitial(selectedPos);
}

void Component::setGenericMap(GenericMap* map, std::string value) 
{
	map->setValue(value);
}






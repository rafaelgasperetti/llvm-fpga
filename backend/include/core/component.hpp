/*
*	-------------------------------------------------------------
*	Defines variables and methods that apply to all components,
*	some virtual.
*	-------------------------------------------------------------
*/

#ifndef COMPONENTE_H
#define	COMPONENTE_H

#include <string>
#include <vector>
#include <unordered_map>

#include "core/port.hpp"
#include "core/genericMap.hpp"

#include <lemon/list_graph.h>

using namespace lemon;
using namespace std;

class Component : ListDigraph::Node{
public:
	// Constructor
	Component();

	// Setters and getters
	void setComponentName(std::string);
	std::string getComponentName();
	void setInstanceName(std::string);
	std::string getInstanceName();
	void setDataWidth(int);
	int getDataWidth();
	virtual bool getIsDeclared() = 0;
	Port* getPort(int);
	Port* getPort(std::string);
	std::vector<Port*> getPorts();
	GenericMap* getMap(int);
	GenericMap* getMap(std::string);
	std::vector<GenericMap*> getMaps();

	// Initialization methods
	virtual void createPorts() = 0;
	virtual void createGenericMaps() = 0;
	//virtual std::string getVHDLDeclaration() = 0;
	virtual std::string getVHDLDeclaration(std::string) = 0;
	void addPort(Port*);
	void addGenericMap(GenericMap*);

	void connectPortSignal(Port*, Signal*);
	void connectPortSignal(Port*, Signal*, std::string);
	void connectPortSignal(Port*, Signal*, std::string, std::string);

	void setGenericMap(GenericMap*, std::string);

	// Hash maps
	std::unordered_map<std::string, int> portStrings;
	std::unordered_map<std::string, int> mapStrings; 

private:
	// Attributes of each component
	//int type;
	std::string componentName;
	std::string instanceName;
	int dataWidth;
	std::vector<Port*> ports;
	std::vector<GenericMap*> maps;
};

#endif	/* COMPONENTE_H */

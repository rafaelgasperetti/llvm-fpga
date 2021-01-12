/*
*	-------------------------------------------------------------
*	A hardware design consists of a Component set and its signals.
*	-------------------------------------------------------------
*/

#ifndef DESIGN_H
#define	DESIGN_H

extern "C" {
	#include "clang-c/Index.h"
}
#include <string>
#include <vector>
#include <unordered_map>
#include "core/component.hpp"
#include "core/signal.hpp"
#include "core/component.hpp"
#include <lemon/list_graph.h>

using namespace lemon;

struct ComponentIdentifier {
	Component* component;
	CXCursor nodeReference; 
};

class Design : ListDigraph::ListDigraph{
public:
    Design();

	// Setters and getters
	ComponentIdentifier getComponent(std::string);
	ComponentIdentifier getComponent(int);
	void addComponent(Component*, CXCursor, std::string);

	// TODO: Verification if name already exists
	void addNamedSignal(Signal*, std::string);
	Signal* createUnnamedSignal(std::string, int);
	Signal* getSignal(std::string);

	void addVariableName(std::string);
	
	// TODO: Change to ComponentIdentifier?
	void connectPortSignal(Component*, Port*, Signal*);
	void connectPortSignal(Component*, Port*, Signal*, std::string);
	void connectPortSignal(Component*, Port*, Signal*, std::string, std::string);

	// TODO: Intermediate fuctions for maps

	// TODO: Put signal generation inside this function, requires treatment for width
	void connectPorts(Component*, Port*, Component*, Port*, Signal*);

	void addToNumberOfComponents(std::string);
	int getNumberOfComponents(std::string);

	// TODO: encapsulate
	std::vector<ComponentIdentifier> components;
	std::vector<Signal*> signals;

private:
	Component* currentParentComponent;

	// For component/signal names generation
	std::vector<int> numberOfComponents;
	int numberOfGeneratedSignals;

	// For storing variable declarations until is possible to decide how to
	// to treat them
	std::vector<std::string> variableNames;

	//Hash maps
	std::unordered_map<std::string, int> componentNames;
	std::unordered_map<std::string, int> signalNames;
	std::unordered_map<std::string, int> componentTypeId;
};

#endif	/* DESIGN_H */

extern "C" {
	#include "clang-c/Index.h"
}
#include <string>
#include <vector>
#include <unordered_map>
#include "core/design.hpp"
#include "core/component.hpp"
#include "core/signal.hpp"
#include "core/design.hpp"
#include <lemon/list_graph.h>

using namespace lemon;

// Class constructor
Design::Design() : ListDigraph::ListDigraph(){
	this->componentTypeId["cmp_add_op_s"] = 0;
    this->componentTypeId["cmp_add_reg_op_s"] = 1;
    this->componentTypeId["cmp_block_ram"] = 2;
    this->componentTypeId["cmp_counter"] = 3;
    this->componentTypeId["cmp_delay_op"] = 4;
    this->componentTypeId["cmp_if_gt_op_s"] = 5;
    this->componentTypeId["cmp_if_it_op_s"] = 6;
    this->componentTypeId["cmp_mult_op_s"] = 7;
    this->componentTypeId["cmp_mux_m"] = 8;
    this->componentTypeId["cmp_neg_op_s"] = 9;
    this->componentTypeId["cmp_reg_mux_m_op"] = 10;
    this->componentTypeId["cmp_reg_op"] = 11;
    this->componentTypeId["cmp_sub_op_s"] = 12;

    for(int i = 0; i < componentTypeId.size(); i++)
        numberOfComponents.push_back(0);

    // Standard signals
    Signal* clkSignal = new Signal("\\clk\\", "std_logic", 1);
    this->addNamedSignal(clkSignal, "clk");
    Signal* resetSignal = new Signal("\\reset\\", "std_logic", 1);
    this->addNamedSignal(resetSignal, "reset");
    Signal* initSignal = new Signal("\\init\\", "std_logic", 1);
    this->addNamedSignal(initSignal, "init");   
    Signal* resultSignal = new Signal("\\result\\", "std_logic", 1);
    this->addNamedSignal(resultSignal, "result");   
    Signal* doneSignal = new Signal("\\done\\", "std_logic", 1);
    this->addNamedSignal(doneSignal, "done");

    this->numberOfGeneratedSignals = 0;
    this->currentParentComponent = NULL;
}

// Setters and getters
void Design::addComponent(Component* component,CXCursor reference, std::string name){
    ComponentIdentifier ci;
    ci.component = component;
    ci.nodeReference = reference;

    this->componentNames[name] = components.size();
    this->components.push_back(ci);
}

ComponentIdentifier Design::getComponent(std::string name){
    return this->components[componentNames[name]];   
}

ComponentIdentifier Design::getComponent(int index){
    return this->components[index];
}

void Design::addNamedSignal(Signal* signal, std::string name){
    this->signalNames[name] = signals.size();
    this->signals.push_back(signal);
}
	
Signal* Design::createUnnamedSignal(std::string type, int width){
    // TODO: Make a better solution than repeated code
    if(!type.compare("std_logic")){
        Signal* newSignal = new Signal("s" + to_string(numberOfGeneratedSignals), "std_logic", 1);
        this->signalNames["s" + to_string(numberOfGeneratedSignals)] = signals.size();
        this->signals.push_back(newSignal);
        numberOfGeneratedSignals++;
        return newSignal;
    }
    else if(!type.compare("std_logic_vector")){
        Signal* newSignal = new Signal("s" + to_string(numberOfGeneratedSignals), "std_logic_vector", width);
        this->signalNames["s" + to_string(numberOfGeneratedSignals)] = signals.size();
        this->signals.push_back(newSignal);
        numberOfGeneratedSignals++;
        return newSignal;
    }
}

Signal* Design::getSignal(std::string name){
    return this->signals[signalNames[name]];
}

void Design::addVariableName(std::string variableName){
    variableNames.push_back(variableName);
}

// Links a port to a signal
void Design::connectPortSignal(Component* component, Port* port, Signal* signal) 
{
	component->connectPortSignal(port, signal);
}

void Design::connectPortSignal(Component* component,
                               Port* port, 
		                       Signal* signal, 
		                       std::string initialWidth, 
		                       std::string finalWidth) 
{
    component->connectPortSignal(port, signal, initialWidth, finalWidth);
}

void Design::connectPortSignal(Component* component,
                               Port* port, 
		                       Signal* signal, 
		                       std::string selectedPos) 
{
    component->connectPortSignal(port, signal, selectedPos);
}

void Design::connectPorts(Component* component_1, 
                          Port* port_1, 
                          Component* component_2, 
                          Port* port_2,
                          Signal* signal){
    this->connectPortSignal(component_1, port_1, signal);
    this->connectPortSignal(component_2, port_2, signal);
}

void Design::addToNumberOfComponents(std::string componentType){
    numberOfComponents[componentTypeId[componentType]]++;
}

int Design::getNumberOfComponents(std::string componentType){
    return numberOfComponents[componentTypeId[componentType]];
}


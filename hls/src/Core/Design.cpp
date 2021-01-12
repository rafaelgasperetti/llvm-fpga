#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>

#include<Core/Component.hpp>
#include<Core/Signal.hpp>
#include<Core/Design.hpp>

#include<Components/cmp_delay_op.hpp>

#include "clang-c/Index.h"

using namespace std;

// Class constructor
Design::Design(){
    Components = new vector<ComponentIdentifier>();
    Signals = new vector<Signal*>();
    Constants = new vector<Signal*>();
    Dependencies = new vector<DependencyLevel*>();

	ComponentTypeId["cmp_add_op_s"] = 0;
    ComponentTypeId["cmp_add_reg_op_s"] = 1;
    ComponentTypeId["cmp_block_ram"] = 2;
    ComponentTypeId["cmp_counter"] = 3;
    ComponentTypeId["cmp_delay_op"] = 4;
    ComponentTypeId["cmp_if_gt_op_s"] = 5;
    ComponentTypeId["cmp_if_it_op_s"] = 6;
    ComponentTypeId["cmp_mult_op_s"] = 7;
    ComponentTypeId["cmp_mux_m"] = 8;
    ComponentTypeId["cmp_neg_op_s"] = 9;
    ComponentTypeId["cmp_reg_mux_m_op"] = 10;
    ComponentTypeId["cmp_reg_op"] = 11;
    ComponentTypeId["cmp_sub_op_s"] = 12;

    for(int i = 0; i < ComponentTypeId.size(); i++)
    {
        NumberOfComponents.push_back(i);
    }

    //Signals used by default
    Signal* clkSignal = new Signal("\\clk\\", "std_logic", 1);
    AddNamedSignal(clkSignal, "clk");
    Signal* resetSignal = new Signal("\\reset\\", "std_logic", 1);
    AddNamedSignal(resetSignal, "reset");
    Signal* initSignal = new Signal("\\init\\", "std_logic", 1);
    AddNamedSignal(initSignal, "init");
    Signal* resultSignal = new Signal("\\result\\", "std_logic_vector", 1);
    AddNamedSignal(resultSignal, "result");
    Signal* doneSignal = new Signal("\\done\\", "std_logic", 1);
    AddNamedSignal(doneSignal, "done");

    NumberOfGeneratedSignals = 0;
}

string Design::GetName()
{
    return Name;
}

void Design::SetName(string name)
{
    Name = name;
}

ComponentIdentifier Design::GetComponent(string name){
    return (*GetComponents())[ComponentNames[name]];
}

ComponentIdentifier Design::GetComponent(int index){
    return (*GetComponents())[index];
}

Component* Design::GetComponent(CXCursor cursor){
    for(ComponentIdentifier ci : (*GetComponents()))
    {
        if(clang_equalCursors(cursor, ci.NodeReference) == 1)
        {
            return ci.IdentifiedComponent;
        }
    }
    return NULL;
}

// Setters and getters
void Design::AddComponent(Component* component, CXCursor reference){
    ComponentIdentifier ci;
    ci.IdentifiedComponent = component;
    ci.NodeReference = reference;

    ComponentNames[component->GetInstanceName()] = (*GetComponents()).size();
    GetComponents()->push_back(ci);
}

void Design::AddNamedSignal(Signal* signal, string name){
    SignalNames[name] = GetSignals()->size();
    GetSignals()->push_back(signal);
}

Signal* Design::CreateUnnamedSignal(string type, int width){
    // TODO: Make a better solution than repeated code
    if(!type.compare("std_logic")){
        Signal* newSignal = new Signal("s" + to_string(NumberOfGeneratedSignals), "std_logic", 1);
        SignalNames["s" + to_string(NumberOfGeneratedSignals)] = GetSignals()->size();
        GetSignals()->push_back(newSignal);
        NumberOfGeneratedSignals++;
        return newSignal;
    }
    else if(!type.compare("std_logic_vector")){
        Signal* newSignal = new Signal("s" + to_string(NumberOfGeneratedSignals), "std_logic_vector", width);
        SignalNames["s" + to_string(NumberOfGeneratedSignals)] = GetSignals()->size();
        GetSignals()->push_back(newSignal);
        NumberOfGeneratedSignals++;
        return newSignal;
    }
}

Signal* Design::GetSignal(string name){
    return (*GetSignals())[SignalNames[name]];
}

void Design::AddNamedConstant(Signal* signal, string name){
    ConstantNames[name] = GetConstants()->size();
    GetConstants()->push_back(signal);
}

Signal* Design::CreateUnnamedConstant(string type, int width){
    // TODO: Make a better solution than repeated code
    if(!type.compare("std_logic")){
        Signal* newSignal = new Signal("s" + to_string(NumberOfGeneratedConstants), "std_logic", 1);
        ConstantNames["s" + to_string(NumberOfGeneratedConstants)] = GetConstants()->size();
        GetConstants()->push_back(newSignal);
        NumberOfGeneratedConstants++;
        return newSignal;
    }
    else if(!type.compare("std_logic_vector")){
        Signal* newSignal = new Signal("s" + to_string(NumberOfGeneratedConstants), "std_logic_vector", width);
        ConstantNames["s" + to_string(NumberOfGeneratedConstants)] = GetConstants()->size();
        GetConstants()->push_back(newSignal);
        NumberOfGeneratedConstants++;
        return newSignal;
    }
}

Signal* Design::GetConstant(string name){
    return (*GetConstants())[ConstantNames[name]];
}

void Design::AddVariableName(string variableName){
    VariableNames.push_back(variableName);
}

vector<string> Design::GetVariableNames(){
    return VariableNames;
}

// Links a port to a signal
void Design::ConnectPortSignal(Component* component, Port* port, Signal* signal, bool sending)
{
	component->ConnectPortSignal(port, signal);
	if(sending)
    {
        signal->ConnectSendingPort(port);
    }
    else
    {
        signal->ConnectReceivingPort(port);
    }
}

void Design::ConnectPortSignal(Component* component, Port* port, Signal* signal, string initialWidth, string finalWidth, bool sending)
{
    component->ConnectPortSignal(port, signal, initialWidth, finalWidth);
    if(sending)
    {
        signal->ConnectSendingPort(port);
    }
    else
    {
        signal->ConnectReceivingPort(port);
    }
}

void Design::ConnectPortSignal(Component* component, Port* port, Signal* signal, string selectedPos, bool sending)
{
    component->ConnectPortSignal(port, signal, selectedPos);
    if(sending)
    {
        signal->ConnectSendingPort(port);
    }
    else
    {
        signal->ConnectReceivingPort(port);
    }
}

void Design::ConnectDelayedPortSignal(Component* component, Port* port, Signal* signal, bool sending, int delayClocks)
{
    if(delayClocks > 0)
    {
        Cmp_delay_op* delay = new Cmp_delay_op(GetWidth());
        delay->GetMap("bits")->SetValue(to_string(GetWidth()));
        delay->GetMap("delay")->SetValue(to_string(delayClocks));

        delay->SetComponentName("delay_op");
        delay->SetInstanceName("from_" + StringUtils::Replace(component->GetInstanceName(),"\\","") + "_to_" + StringUtils::Replace(signal->GetSignalName(),"\\",""));

        AddDeclaredName(delay->GetInstanceName());
        AddComponent(delay, GetComponent(component->GetInstanceName()).NodeReference);
        AddToNumberOfComponents(delay->GetInstanceName());

        ConnectPortSignal(delay, delay->GetPort("clk"), GetSignal("\\clk\\"), false);
        ConnectPortSignal(delay, delay->GetPort("reset"), GetSignal("\\reset\\"), false);

        Signal* intermediate = new Signal("\\inter_from_" + StringUtils::Replace(component->GetInstanceName(),"\\","") + "_" + port->GetName() + "_" + StringUtils::Replace(signal->GetSignalName(),"\\","") + "\\","std_logic_vector",GetWidth());

        ConnectPorts(component, port, delay, delay->GetPort("a"), intermediate);
        component->ConnectPortSignal(delay->GetPort("a_delayed"), signal);
    }
    else
    {
        component->ConnectPortSignal(port, signal);
    }


	if(sending)
    {
        signal->ConnectSendingPort(port);
    }
    else
    {
        signal->ConnectReceivingPort(port);
    }
}

void Design::ConnectPorts(Component* component_from, Port* port_from, Component* component_to, Port* port_to, Signal* signal){
    ConnectPortSignal(component_from, port_from, signal, true);
    ConnectPortSignal(component_to, port_to, signal, false);
    //signal->ConnectSendingPort(port_from);
    //signal->ConnectReceivingPort(port_to);
}

void Design::ConnectDelayedPorts(Component* component_from, Port* port_from, Component* component_to, Port* port_to, Signal* signal, int delayClocks){

    if(delayClocks > 0)
    {
        Cmp_delay_op* delay = new Cmp_delay_op(GetWidth());
        delay->GetMap("bits")->SetValue(to_string(GetWidth()));
        delay->GetMap("delay")->SetValue(to_string(delayClocks));

        delay->SetComponentName("delay_op");
        delay->SetInstanceName("delay_from_" + StringUtils::Replace(component_from->GetInstanceName(),"\\","") + "_to_" + StringUtils::Replace(component_to->GetInstanceName(),"\\",""));

        AddDeclaredName(delay->GetInstanceName());
        AddComponent(delay, GetComponent(component_from->GetInstanceName()).NodeReference);
        AddToNumberOfComponents(delay->GetInstanceName());

        delay->ConnectDefaultSignals(this);

        Signal* intermediate = new Signal("\\inter_from_" + StringUtils::Replace(component_from->GetInstanceName(),"\\","") + "_" + port_from->GetName() + "_to_" + StringUtils::Replace(component_to->GetInstanceName(),"\\","") + "_" + port_to->GetName() + "\\","std_logic_vector",GetWidth());

        ConnectPorts(component_from, port_from, delay, delay->GetPort("a"), intermediate);
        ConnectPorts(delay, delay->GetPort("a_delayed"), component_to, port_to, signal);

        cout << "Added delay " << GetComponent(delay->GetInstanceName()).IdentifiedComponent->GetInstanceName() << " between " << component_from->GetInstanceName() << " and " << component_to->GetInstanceName() << endl;
    }
    else
    {
        ConnectPortSignal(component_from, port_from, signal, true);
        ConnectPortSignal(component_to, port_to, signal, false);
    }

}

int Design::GetNumberOfComponents(string componentType){
    return NumberOfComponents[ComponentTypeId[componentType]];
}

void Design::AddToNumberOfComponents(string componentType){
    NumberOfComponents[ComponentTypeId[componentType]]++;
}

void Design::AddDeclaredName(string name)
{
    DeclaredNames.push_back(name);
}

string Design::GetDeclaredName(int index)
{
    return DeclaredNames[index];
}

vector<string> Design::GetDeclaredNames()
{
    return DeclaredNames;
}

void Design::IncrementNumberOfFors()
{
    NumberOfFors++;
}

int Design::GetNumberOfFors()
{
    return NumberOfFors;
}

void Design::IncrementNumberOfOperations()
{
    NumberOfOperations++;
}

int Design::GetNumberOfOperations()
{
    return NumberOfOperations;
}

void Design::IncrementNumberOfFunctions()
{
    NumberOfFunctions++;
}

int Design::GetNumberOfFunctions()
{
    return NumberOfFunctions;
}

vector<ComponentIdentifier>* Design::GetComponents()
{
    return Components;
}

vector<Signal*>* Design::GetSignals()
{
    return Signals;
}

vector<Signal*>* Design::GetConstants()
{
    return Constants;
}

int Design::GetWidth()
{
    return Width;
}

void Design::SetWidth(int width)
{
    if(width > 0)
    {
        Width = width;
        GetSignal("result")->SetDataWidth(Width);
    }
    else
    {
        Width = 32;
    }
}

bool Design::HasComponent(string name)
{
    for(ComponentIdentifier c : (*GetComponents()))
    {
        if(c.IdentifiedComponent->GetInstanceName().compare(name) == 0)
        {
            return true;
        }
    }
    return NULL;
}

void Design::AddDependencyLevel(DependencyLevel* dep)
{
    if(GetDependencyLevel(dep->GetLevel(), dep->GetType()) == NULL)
    {
        Dependencies->push_back(dep);
    }
    else
    {
        throw invalid_argument("A NULL dependency level must never be added to the design.");
    }
}

DependencyLevel* Design::GetDependencyLevel(int lvl, int type)
{
    for(DependencyLevel* depLvl : (*Dependencies))
    {
        if(depLvl->GetLevel() == lvl && depLvl->GetType() == type)
        {
            return depLvl;
        }
    }
    return NULL;
}

DependencyLevel* Design::GetAndCreateDependencyLevel(int lvl, int type)
{
    DependencyLevel* dpLvl = GetDependencyLevel(lvl, type);
    if(dpLvl == NULL)
    {
        dpLvl = new DependencyLevel(lvl, type);
        AddDependencyLevel(dpLvl);
        cout << "Added dependency level " << dpLvl->GetLevel() << " of type " << type << endl;
    }
    else
    {
        cout << "Used dependency level " << dpLvl->GetLevel() << " of type " << type << endl;
    }
    return dpLvl;
}

vector<DependencyLevel*>* Design::GetDependencyLevel(Component* comp)
{
    vector<DependencyLevel*>* ret = new vector<DependencyLevel*>;
    if(comp != NULL)
    {
        for(DependencyLevel* lvl : (*Dependencies))
        {
            if(lvl->GetDependency(comp->GetInstanceName()) != NULL)
            {
                ret->push_back(lvl);
            }
        }
    }
    return ret;
}

vector<DependencyLevel*>* Design::GetDependencies()
{
    return Dependencies;
}

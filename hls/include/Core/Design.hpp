#ifndef DESIGN_H
#define	DESIGN_H

#include<string>
#include<vector>
#include<unordered_map>
#include<iostream>

#include<Tools/StringUtils.hpp>

#include<Core/Component.hpp>
#include<Core/Signal.hpp>

extern "C" {
	#include "clang-c/Index.h"
}

using namespace std;

class DependencyLevel;
class Component;
class Signal;
class Port;

struct ComponentIdentifier {
	Component* IdentifiedComponent;
	CXCursor NodeReference;
};

class Design
{
    public:
        Design();
        // Setters and Getters

        string GetName();
        void SetName(string);

        ComponentIdentifier GetComponent(string);
        ComponentIdentifier GetComponent(int);
        Component* GetComponent(CXCursor);
        bool HasComponent(string);
        void AddComponent(Component*, CXCursor);

        // TODO: Verification if name already exists
        void AddNamedSignal(Signal*, string);
        void AddNamedConstant(Signal*, string);
        Signal* CreateUnnamedSignal(string, int);
        Signal* CreateUnnamedConstant(string, int);
        Signal* GetSignal(string);
        Signal* GetConstant(string);

        void AddVariableName(string);
        vector<string> GetVariableNames();

        // TODO: Change to ComponentIdentifier?
        void ConnectPortSignal(Component*, Port*, Signal*, bool);
        void ConnectPortSignal(Component*, Port*, Signal*, string, bool);
        void ConnectPortSignal(Component*, Port*, Signal*, string, string, bool);

        void ConnectDelayedPortSignal(Component*, Port*, Signal*, bool, int);

        // TODO: Intermediate fuctions for maps

        // TODO: Put signal generation inside this function, requires treatment for width
        void ConnectPorts(Component*, Port*, Component*, Port*, Signal*);
        void ConnectDelayedPorts(Component*, Port*, Component*, Port*, Signal*, int);

        void AddToNumberOfComponents(string);
        int GetNumberOfComponents(string);

        void AddDeclaredName(string);
        string GetDeclaredName(int);
        vector<string> GetDeclaredNames();

        // TODO: encapsulate
        vector<ComponentIdentifier>* GetComponents();
        vector<Signal*>* GetSignals();
        vector<Signal*>* GetConstants();

        void IncrementNumberOfFors();
        int GetNumberOfFors();

        int GetWidth();
        void SetWidth(int);

        void IncrementNumberOfOperations();
        int GetNumberOfOperations();

        void IncrementNumberOfFunctions();
        int GetNumberOfFunctions();

        void AddDependencyLevel(DependencyLevel*);
        DependencyLevel* GetDependencyLevel(int,int);
        DependencyLevel* GetAndCreateDependencyLevel(int,int);
        vector<DependencyLevel*>* GetDependencyLevel(Component*);
        vector<DependencyLevel*>* GetDependencies();

    private:
        string Name;

        vector<ComponentIdentifier>* Components;
        vector<Signal*>* Signals;
        vector<Signal*>* Constants;
        vector<DependencyLevel*>* Dependencies;

        // For component/signal names generation
        vector<int> NumberOfComponents;
        int NumberOfGeneratedSignals = 0;
        int NumberOfGeneratedConstants = 0;
        int NumberOfFors = 0;
        int NumberOfOperations = 0;
        int NumberOfFunctions = 0;
        int Width = 32;

        // For storing variable declarations until is possible to decide how to
        // to treat them
        vector<string> VariableNames;

        //Hash maps
        vector<string> DeclaredNames;
        unordered_map<string, int> ComponentNames;
        unordered_map<string, int> SignalNames;
        unordered_map<string, int> ConstantNames;
        unordered_map<string, int> ComponentTypeId;
};

#endif	/* DESIGN_H */

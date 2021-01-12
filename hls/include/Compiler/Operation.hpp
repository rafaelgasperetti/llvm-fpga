#ifndef OPERATION_H
#define	OPERATION_H

#include<iostream>
#include<string>
#include<vector>

#include<Compiler/Context.hpp>
#include<Compiler/TimingControl.hpp>

#include<Core/Component.hpp>

extern "C" {
	#include<clang-c/Index.h>
}

class Component;
class DependencyLevel;
class Context;

using namespace std;
class Operation : public TimingControl
{
    private:
        string Name;
        Component* RelatedComponent;
        vector<pair<Component*, int>*>* Dependencies = new vector<pair<Component*, int>*>();//The component dependency and the Dependencies address case it's a subdependency of another dependency, like the 'i' of a[i], for example. -1 if is not.
        CXCursor RelatedCursor;
        DependencyLevel* Level;
        Context* ParentContext;

    public:
        Operation(string, Component*, CXCursor);
        void SetName(string);
        string GetName();
        void SetRelatedComponent(Component*);
        Component* GetRelatedComponent();
        void AddDependency(Component*, int);
        Component* GetDependency(string);
        Component* GetDependency(string, int);
        pair<Component*, int>* GetFullDependency(string);
        pair<Component*, int>* GetFullDependency(string, int);
        int GetDependencyAddress(string, int);
        vector<pair<Component*, int>*>* GetDependencies();
        void SetRelatedCursor(CXCursor);
        CXCursor GetRelatedCursor();
        void SetDependencyLevel(DependencyLevel*);
        DependencyLevel* GetDependencyLevel();
        void SetContext(Context*);
        Context* GetContext();
        bool IsBoolean();
        bool IsNumeric();
        bool IsAttribution();
        bool IsCompound();

        static void ChangeDependency(vector<Operation*>*, string, Component*);
        static bool IsCompoundOperation(string);
};

#endif

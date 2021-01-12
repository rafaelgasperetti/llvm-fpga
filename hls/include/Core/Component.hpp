#ifndef COMPONENT_H
#define	COMPONENT_H

#include<string>
#include<vector>
#include<unordered_map>

#include<Core/Port.hpp>
#include<Core/Signal.hpp>
#include<Core/GenericMap.hpp>
#include<Core/Design.hpp>

#include<Compiler/Context.hpp>
#include<Compiler/DependencyLevel.hpp>
#include<Compiler/TimingControl.hpp>

using namespace std;

class Port;
class Signal;
class Design;

class Component : public TimingControl
{
    public:
        Component();
        // Setters and getters
        void SetComponentName(string);
        string GetComponentName();
        void SetInstanceName(string);
        string GetInstanceName();
        void SetDataWidth(int);
        int GetDataWidth();
        void SetDeclared(bool);
        bool IsIoComponent();
        bool IsBlockRam();
        bool IsDeclared();
        Port* GetPort(int);
        Port* GetPort(string);
        vector<Port*> GetPorts();
        GenericMap* GetMap(int);
        GenericMap* GetMap(string);
        vector<GenericMap*> GetMaps();
        /*void SetClockEnable(int);
        int GetClockEnable();
        void SetClockDelay(int);
        int GetClockDelay();*/
        void SetUniversal(bool);
        bool IsUniversal();
        bool HasPort(string);
        void SetId(int);
        int GetId();
        void SetContext(Context*);
        Context* GetContext();
        void ConnectDefaultSignals(Design*);

        // Initialization methods
        virtual void CreatePorts() = 0;
        virtual void CreateGenericMaps() = 0;
        virtual string GetVHDLDeclaration(string) = 0;

        void AddPort(Port*);
        void AddGenericMap(GenericMap*);

        void ConnectPortSignal(Port*, Signal*);
        void ConnectPortSignal(Port*, Signal*, string);
        void ConnectPortSignal(Port*, Signal*, string, string);

        void SetGenericMap(GenericMap*, string);

        // Hash maps
        unordered_map<string, int> PortStrings;
        unordered_map<string, int> MapStrings;

        vector<string> AssignedSignals;
        vector<string> AssignedConstants;

        static string GetBinaryValue(long long int, int);
        static string GetBinaryValue(int, int);

        static int GetIntegerValue(string);
        static long long int GetLongValue(string);

        void AddDependencyLevel(DependencyLevel*);
        void RemoveDependencyLevel(int, int);
        void AddDependencyLevels(vector<DependencyLevel*>);
        DependencyLevel* GetDependencyLevel(int,int);
        DependencyLevel* GetMaxDependencyLevel(int);
        DependencyLevel* GetMaxDependencyLevel();
        vector<DependencyLevel*> GetDependencyLevels();

        void AddDependent(Component*);
        Component* GetDependent(string);

        void SetParent(Component*);
        Component* GetParent();
    protected:
        // Attributes of each component
        //int type;
        string ComponentName;
        string InstanceName;
        int DataWidth;
        vector<Port*> Ports;
        vector<GenericMap*> Maps;
        bool Declared;
        bool IoComponent = false;
        bool BlockRam = false;
        bool Universal = false;
        vector<DependencyLevel*> Levels;
        vector<Component*> Dependents;
        Component* Parent;
        int Id = -1;
        Context* ParentContext;
};
#endif	/* COMPONENTE_H */

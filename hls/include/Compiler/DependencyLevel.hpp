#ifndef DEPENDENCYLEVEL_H
#define	DEPENDENCYLEVEL_H

#include<Core/Component.hpp>
#include<Compiler/Operation.hpp>

class Component;
class Operation;
class Context;
class DependencyLevel;

class DependencyLevel
{
    private:
            int Level = -1;
            int Type = -1;
            int CycleNumbers = 1;
            vector<Component*> Dependencies;

        public:
            DependencyLevel(int, int);
            void SetLevel(int);
            int GetLevel();
            void AddDependency(Component*);
            void RemoveDependency(string);
            Component* GetDependency(string);
            vector<Component*> GetDependencies();
            bool Contains(Component*);
            bool Contains(string);
            int IndexOf(Component*);
            int IndexOf(string);
            void SetType(int);
            int GetType();
            void SetCycleNumbers(int);
            int GetCycleNumbers();

            static void MoveDependency(DependencyLevel*,DependencyLevel*, string);
            static void ReplicateDependency(DependencyLevel*,DependencyLevel*, string);
            static void ManageDependencyLevels(DependencyLevel*, Operation*, vector<Operation*>*);

            static int TYPE_READ;
            static int TYPE_WRITE;
};

#endif

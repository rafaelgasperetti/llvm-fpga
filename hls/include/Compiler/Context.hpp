#ifndef CONTEXT_H
#define	CONTEXT_H

#include<iostream>
#include<string>
#include<vector>

#include<Compiler/NodeInfo.hpp>
#include<Compiler/Operation.hpp>
#include<Compiler/TimingControl.hpp>

extern "C" {
	#include<clang-c/Index.h>
}

using namespace std;

class NodeInfo;

class Context : public TimingControl
{
    private:
        string Name;
        CXCursor Cursor;
        int Depth;
        bool CompoundStatement;
        Context* Parent;
        vector<Context*>* Children = new vector<Context*>;
        Operation* LoopCondition;
        vector<pair<Component*,int>*>* StartValues = new vector<pair<Component*,int>*>;
        vector<pair<Component*,int>*>* IncrementRates = new vector<pair<Component*,int>*>;

    public:
        Context();
        Context(string);
        string GetName();
        void SetName(string);
        CXCursor GetCursor();
        void SetCursor(CXCursor);
        int GetDepth();
        void SetDepth(int);
        bool IsCompoundStatement();
        void SetCompoundStatement(bool);
        Context* GetParent();
        void SetParent(Context*);
        vector<Context*>* GetChildren();
        int FindChild(string);
        int FindChild(string, int);
        Context* GetChild(string);
        Context* GetChild(int);
        Context* RemoveChild(string);
        Context* RemoveChild(int);
        Context* PopChild();
        void AddChild(string, bool, CXCursor, Context*, int);
        bool IsLoop();
        void SetLoopCondition(Operation*);
        Operation* GetLoopCondition();
        void AddStartValue(Component*, int);
        pair<Component*,int>* GetStartValue(Component*);
        vector<pair<Component*,int>*>* GetStartValues();
        void AddIncrementRate(Component*, int);
        pair<Component*,int>* GetIncrementRate(Component*);
        vector<pair<Component*,int>*>* GetIncrementRates();
        int GetLoopClockCount();
        int GetContextTreeClycleCount();

        static void AddContext(vector<Context*>*, string, bool, CXCursor, Context*, int);
        static int FindContext(vector<Context*>*, CXCursor);
        static int FindContext(vector<Context*>*, string);
        static int FindContext(vector<Context*>*, string, int);
        static Context* GetContext(vector<Context*>*, CXCursor);
        static Context* GetContext(vector<Context*>*, string);
        static Context* GetContext(vector<Context*>*, int);
        static Context* GetLastContext(vector<Context*>*);
        static Context* RemoveContext(vector<Context*>*, string);
        static Context* RemoveContext(vector<Context*>*, int);
        static Context* PopContext(vector<Context*>*);
        static void CheckAddContext(NodeInfo*, vector<Context*>*);
        static void CheckRemoveContext(NodeInfo*, vector<Context*>*);
        static void PrintContext(vector<Context*>* contexts);
};

#endif

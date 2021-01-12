#ifndef NODEVISITOR_H
#define	NODEVISITOR_H

#include<iostream>
#include<string>
#include<vector>

#include<Compiler/Operation.hpp>

#include<Core/Design.hpp>
#include<Core/Component.hpp>
#include<Core/Signal.hpp>

#include<Components/cmp_add_reg_op_s.hpp>
#include<Components/cmp_sub_op_s.hpp>
#include<Components/cmp_block_ram.hpp>
#include<Components/cmp_counter.hpp>
#include<Components/cmp_delay_op.hpp>
#include<Components/cmp_mult_op_s.hpp>
#include<Components/cmp_reg_op.hpp>
#include<Components/cmp_counter.hpp>
#include<Components/cmp_const_op.hpp>

#include<Compiler/Context.hpp>
#include<Compiler/Variable.hpp>

extern "C" {
	#include<clang-c/Index.h>
}

using namespace std;

class NodeVisitor
{
    private:
        //Variables
        Design* VisitorDesign;
        CXTranslationUnit TranslationUnit;
        vector<string> AssignedSignals;
        vector<string> DeclaredNames;
        //Cmp_counter* Counter;
        //Cmp_add_reg_op_s* Add_reg;
        vector<Variable*>* Variables = new vector<Variable*>;
        vector<Context*>* Contexts = new vector<Context*>;
        vector<NodeInfo*>* InfoHistory = new vector<NodeInfo*>;//Represents the history of the tree's cursor.
        vector<Operation*>* Operations = new vector<Operation*>;

        //Methods
        CXTranslationUnit Initialize(char*);

        static CXChildVisitResult VisitNode(CXCursor, CXCursor, CXClientData);//Return and parameters cannot be changed
        static CXChildVisitResult FirstVisitation(CXCursor, CXCursor, CXClientData);//Return and parameters cannot be changed
        static CXChildVisitResult DependencyVisitation(CXCursor, CXCursor, CXClientData);//Return and parameters cannot be changed

        static CXChildVisitResult ChooseVisitor(NodeInfo*);
        static CXChildVisitResult VisitFirstExpression(NodeInfo*);
        static CXChildVisitResult VisitUnexposedExpression(NodeInfo*);
        static CXChildVisitResult VisitBinaryOperator(NodeInfo*);
        static CXChildVisitResult VisitDeclaration(NodeInfo*);
        static CXChildVisitResult VisitVariable(NodeInfo*);
        static CXChildVisitResult VisitIntCounter(NodeInfo*);
        static CXChildVisitResult VisitIntLiteral(NodeInfo*);
        static CXChildVisitResult VisitFunction(NodeInfo*);
        static CXChildVisitResult VisitFor(NodeInfo*);
        static CXChildVisitResult VisitIf(NodeInfo*);
        static CXChildVisitResult VisitDeclarationReference(NodeInfo*);
        static CXChildVisitResult VisitReturn(NodeInfo*);
        static CXChildVisitResult VisitCompoundStatement(NodeInfo*);
        static CXChildVisitResult VisitArraySubscript(NodeInfo*);
        static CXChildVisitResult VisitCompoundAssignOperator(NodeInfo*);

        static vector<Cmp_const_op*> GetConstants(NodeInfo*);
        static void VisitUnaryOperator(NodeInfo*);
        static Component* GetOperationDependency(string, vector<Operation*>*);

    public:
        void SetDesign(Design*);
        Design* GetDesign();
        void SetTranslationUnit(CXTranslationUnit);
        CXTranslationUnit GetTranslationUnit();
        static Design* VisitAll(char*, int);

        void AddVariable(string, Component*, CXCursor);
        vector<Variable*>* GetVariables();
        vector<NodeInfo*>* GetInfoHistory();
        void ClearInfoHistory();
        void AddInfoHistory(NodeInfo*);
        Variable* GetVariable(string);
        Variable* GetVariable(CXCursor);
        void ClearContexts();

        vector<Context*>* GetContexts();

        void AddOperation(Operation*);
        Operation* GetOperation(string);
        Operation* GetOperation(CXCursor);
        vector<Operation*>* GetOperations();

        void SetCurrentCompoundStatementCursor(CXCursor);
        CXCursor GetCurrentCompoundStatementCursor();
};

#endif // NODEVISITOR_H

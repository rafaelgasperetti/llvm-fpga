#ifndef NODEINFO_H
#define	NODEINFO_H

#include<iostream>
#include<string>
#include<vector>

#include<Core/Design.hpp>
#include<Core/Component.hpp>
#include<Core/Signal.hpp>

extern "C" {
	#include<clang-c/Index.h>
}

using namespace std;

class Context;

class NodeInfo
{
    private:
        CXCursor Cursor;
        CXCursor ParentCursor;
        vector<NodeInfo*>* CursorHistory = new vector<NodeInfo*>;
        int* ClientData;

    public:
        NodeInfo(CXCursor, CXCursor, CXClientData);
        void SetCursor(CXCursor);
        CXCursor GetCursor();
        void SetParentCursor(CXCursor);
        CXCursor GetParentCursor();
        CXCursorKind GetCursorKind();
        CXType GetCursorType();
        CXTypeKind GetCursorTypeKind();
        CXCursorKind GetParentCursorKind();
        CXType GetParentCursorType();
        CXTypeKind GetParentCursorTypeKind();
        string GetName();
        string GetParentName();
        CXSourceLocation GetCursorLocation();
        CXSourceLocation GetParentCursorLocation();
        void SetClientData(CXClientData);
        int* GetClientData();
        int* GetNextClientData();
        bool IsFromMainFile();
        bool IsParentFromMainFile();
        unsigned GetCursorHash();
        unsigned GetParentCursorHash();
        void SetCursorHistory(vector<NodeInfo*>*);
        vector<NodeInfo*>* GetCursorHistory();
        NodeInfo* GetCursorHistory(int);//Positive numbers counts from 0 to the number. Negative numbers counts from the last element decreasing the number to the quantity.
        int GetCursorDepth();
        int GetParentCursorDepth();
        vector<CXToken> GetCursorTokens();
        vector<CXToken> GetParentCursorTokens();

        static string GetCursorKindName(CXCursorKind);
        static Component* GetComponent(string, int);
        static Component* GetCompoundComponent(string, int);

        static void Push(vector<NodeInfo*>*, NodeInfo*);
        static NodeInfo* GetLast(vector<NodeInfo*>*);
        static NodeInfo* Pop(vector<NodeInfo*>*);
        static NodeInfo* Remove(vector<NodeInfo*>*, int);
        static NodeInfo* Get(vector<NodeInfo*>*, int);
        static int Get(vector<NodeInfo*>*, CXCursor);
        static vector<NodeInfo*>* GetPreviousHistory(vector<NodeInfo*>*, CXCursor);

        static void PrintTokenInfo(CXTranslationUnit, CXToken);
        static void PrintCursorTokens( CXCursor);
        static string GetTokenName(CXTranslationUnit, CXToken);
        static string GetTokenFromCursor(CXCursor, int);
        static string GetOperation(CXCursor);
        static vector<CXToken> GetTokensFromCursor(CXCursor);
        static bool IsArray(CXType);
        static void PrintHistory(vector<NodeInfo*>*);
        static void PrintCursorInfo(string, NodeInfo* info);
        static void PrintCursorInfo(NodeInfo* info);
        static void PrintHistoryNames(vector<NodeInfo*>*);

        static const int History_1_Level_Above = 0;
        static const int History_2_Levels_Above = 1;
        static const int History_3_Levels_Above = 2;
        static const int History_4_Levels_Above = 3;
        static const int History_First_Level = -1;
        static const int History_Second_Level = -2;
        static const int History_Third_Level = -3;
        static const int History_Fourth_Level = -4;

        static const string Operation_UnknownOperation;
        static const string Operation_LessThan;
        static const string Operation_LessEqualThan;
        static const string Operation_Equal;
        static const string Operation_GreaterEqualThan;
        static const string Operation_GreaterThan;
        static const string Operation_Different;
        static const string Operation_Sum;
        static const string Operation_CompoundSum;
        static const string Operation_CompoundSum2;
        static const string Operation_Subtraction;
        static const string Operation_CompoundSubtraction;
        static const string Operation_CompoundSubtraction2;
        static const string Operation_Multiplication;
        static const string Operation_CompoundMultiplication;
        static const string Operation_Division;
        static const string Operation_CompoundDivision;
        static const string Operation_Mod;
        static const string Operation_Attribution;

        static const string Operation_And;
        static const string Operation_Or;
        static const string Operation_Not;
};

#endif // NODEINFO_H

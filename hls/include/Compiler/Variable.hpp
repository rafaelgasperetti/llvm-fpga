#ifndef VARIABLE_H
#define	VARIABLE_H


#include<iostream>
#include<string>

#include<Core/Component.hpp>

extern "C" {
	#include<clang-c/Index.h>
}

class Variable
{
    private:
        string Name;
        Component* RelatedComponent;
        CXCursor RelatedCursor;

    public:
        Variable(string);
        string GetName();
        void SetName(string);
        Component* GetRelatedComponent();
        void SetRelatedComponent(Component*);
        CXCursor GetRelatedCursor();
        void SetRelatedCursor(CXCursor);
};

#endif

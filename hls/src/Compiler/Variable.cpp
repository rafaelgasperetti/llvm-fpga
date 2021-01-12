#include<Compiler/Variable.hpp>

Variable::Variable(string name)
{
    SetName(name);
}

string Variable::GetName()
{
    return Name;
}

void Variable::SetName(string name)
{
    Name = name;
}

Component* Variable::GetRelatedComponent()
{
    return RelatedComponent;
}

void Variable::SetRelatedComponent(Component* relatedComponent)
{
    RelatedComponent = relatedComponent;
}

CXCursor Variable::GetRelatedCursor()
{
    return RelatedCursor;
}

void Variable::SetRelatedCursor(CXCursor relatedCursor)
{
    RelatedCursor = relatedCursor;
}

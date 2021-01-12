#include<Compiler/Operation.hpp>
#include<Compiler/NodeInfo.hpp>

Operation::Operation(string name, Component* relatedComponent, CXCursor cursor)
{
    Level = NULL;
    SetName(name);
    SetRelatedComponent(relatedComponent);
    SetRelatedCursor(cursor);
}

void Operation::SetName(string name)
{
    Name = name;
}

string Operation::GetName()
{
    return Name;
}

void Operation::SetRelatedComponent(Component* comp)
{
    if
    (
        (GetName().compare(NodeInfo::Operation_Attribution) == 0 && comp == NULL) ||
        (GetName().compare(NodeInfo::Operation_CompoundSum) == 0 && comp == NULL) ||
        (GetName().compare(NodeInfo::Operation_CompoundSubtraction) == 0 && comp == NULL) ||
        (
            GetName().compare(NodeInfo::Operation_Attribution) != 0 &&
            GetName().compare(NodeInfo::Operation_CompoundSum) != 0 &&
            GetName().compare(NodeInfo::Operation_CompoundSubtraction) != 0 &&
            GetName().compare(NodeInfo::Operation_CompoundMultiplication) != 0 &&
            GetName().compare(NodeInfo::Operation_CompoundDivision) != 0 &&
            comp != NULL
        )
    )
    {
        RelatedComponent = comp;
    }
    else
    {
        throw invalid_argument("Null related component are only allowed at operations '=', '++', '--'. Otherwise it must be filled. Current operation: '" + GetName() + "'.");
    }
}

Component* Operation::GetRelatedComponent()
{
    return RelatedComponent;
}

void Operation::AddDependency(Component* comp, int subdependency)
{
    if(comp!= NULL && GetDependency(comp->GetInstanceName(), subdependency) == NULL)
    {
        Dependencies->push_back(new pair<Component*,int>(comp, subdependency));
    }
    else
    {
        throw invalid_argument("Repeated or null dependencies on Operation are not allowed.");
    }
}

Component* Operation::GetDependency(string compName)
{
    for(pair<Component*,int>* c : (*Dependencies))
    {
        if(c->first->GetInstanceName().compare(compName) == 0)
        {
            return c->first;
        }
    }
    return NULL;
}

Component* Operation::GetDependency(string compName, int addr)
{
    for(pair<Component*,int>* c : (*Dependencies))
    {
        if(c->first->GetInstanceName().compare(compName) == 0 && c->second == addr)
        {
            return c->first;
        }
    }
    return NULL;
}

pair<Component*, int>* Operation::GetFullDependency(string compName)
{
    for(pair<Component*,int>* c : (*Dependencies))
    {
        if(c->first->GetInstanceName().compare(compName) == 0)
        {
            return c;
        }
    }
    return NULL;
}

pair<Component*, int>* Operation::GetFullDependency(string compName, int addr)
{
    for(pair<Component*,int>* c : (*Dependencies))
    {
        if(c->first->GetInstanceName().compare(compName) == 0 && c->second == addr)
        {
            return c;
        }
    }
    return NULL;
}

int Operation::GetDependencyAddress(string compName, int addr)
{
    for(int i = 0; i < Dependencies->size(); i++)
    {
        if((*Dependencies)[i]->first->GetInstanceName().compare(compName) == 0 && (*Dependencies)[i]->second == addr)
        {
            return i;
        }
    }
    return -1;
}

vector<pair<Component*,int>*>* Operation::GetDependencies()
{
    return Dependencies;
}

void Operation::SetRelatedCursor(CXCursor cursor)
{
    RelatedCursor = cursor;
}

CXCursor Operation::GetRelatedCursor()
{
    return RelatedCursor;
}

void Operation::SetDependencyLevel(DependencyLevel* level)
{
    /*for(pair<Component*,int>* dep : (*Dependencies))
    {
        if(Level != NULL && Level->GetDependency(dep->first->GetInstanceName()) != NULL && level->GetDependency(dep->first->GetInstanceName()) == NULL)
        {
            DependencyLevel::MoveDependency(Level, level, dep->first->GetInstanceName());
        }
        else
        {
            if(level->GetDependency(dep->first->GetInstanceName()) == NULL)
            {
                level->AddDependency(dep->first);
            }
            cout << "Added dependency level " << dep->first->GetDependencyLevel(level->GetLevel(), level->GetType())->GetLevel() << " to " << dep->first->GetInstanceName() << endl;
        }
    }*/

    Level = level;
}

DependencyLevel* Operation::GetDependencyLevel()
{
    return Level;
}

void Operation::ChangeDependency(vector<Operation*>* ops, string name, Component* comp)
{
    for(Operation* op : (*ops))
    {
        pair<Component*, int>* dep = op->GetFullDependency(name);
        if(dep != NULL)
        {
            cout << "Changed dependency at operation " << op->GetName() << " from " << dep->first->GetInstanceName() << " (" << dep->first->GetComponentName()
                << ") to " << comp->GetInstanceName() << " (" << comp->GetComponentName() << ")" << endl;
            dep->first = comp;
        }
    }
}

void Operation::SetContext(Context* context)
{
    ParentContext = context;
}

Context* Operation::GetContext()
{
    return ParentContext;
}

bool Operation::IsBoolean()
{
    return  GetName().compare(NodeInfo::Operation_And) == 0 ||
            GetName().compare(NodeInfo::Operation_Or) == 0 ||
            GetName().compare(NodeInfo::Operation_Not) == 0 ||
            GetName().compare(NodeInfo::Operation_GreaterThan) == 0 ||
            GetName().compare(NodeInfo::Operation_GreaterEqualThan) == 0 ||
            GetName().compare(NodeInfo::Operation_LessThan) == 0 ||
            GetName().compare(NodeInfo::Operation_LessEqualThan) == 0 ||
            GetName().compare(NodeInfo::Operation_Equal) == 0 ||
            GetName().compare(NodeInfo::Operation_Different) == 0;
}

bool Operation::IsNumeric()
{
    return  GetName().compare(NodeInfo::Operation_Sum) == 0 ||
            GetName().compare(NodeInfo::Operation_Subtraction) == 0 ||
            GetName().compare(NodeInfo::Operation_Multiplication) == 0 ||
            GetName().compare(NodeInfo::Operation_Division) == 0 ||
            GetName().compare(NodeInfo::Operation_Mod) == 0 ||
            GetName().compare(NodeInfo::Operation_CompoundSum) == 0 ||
            GetName().compare(NodeInfo::Operation_CompoundSum2) == 0 ||
            GetName().compare(NodeInfo::Operation_CompoundSubtraction) == 0 ||
            GetName().compare(NodeInfo::Operation_CompoundSubtraction2) == 0 ||
            GetName().compare(NodeInfo::Operation_CompoundMultiplication) == 0 ||
            GetName().compare(NodeInfo::Operation_CompoundDivision) == 0;
}

bool Operation::IsAttribution()
{
    return GetName().compare(NodeInfo::Operation_Attribution) == 0;
}

bool Operation::IsCompound()
{
    return  IsCompoundOperation(GetName());
}

bool Operation::IsCompoundOperation(string operation)
{
    return  operation.compare(NodeInfo::Operation_CompoundSum) == 0 ||
            operation.compare(NodeInfo::Operation_CompoundSum2) == 0 ||
            operation.compare(NodeInfo::Operation_CompoundSubtraction) == 0 ||
            operation.compare(NodeInfo::Operation_CompoundSubtraction2) == 0 ||
            operation.compare(NodeInfo::Operation_CompoundMultiplication) == 0 ||
            operation.compare(NodeInfo::Operation_CompoundDivision) == 0;
}

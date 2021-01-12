#include<Compiler/Context.hpp>
#include<Core/IOComponent.hpp>

Context::Context()
{
    SetDepth(0);
    SetLoopCondition(NULL);
    SetParent(NULL);
}

Context::Context(string name)
{
    SetName(name);
    SetDepth(0);
    SetParent(NULL);
}

string Context::GetName()
{
    return Name;
}

void Context::SetName(string name)
{
    Name = name;
}

CXCursor Context::GetCursor()
{
    return Cursor;
}

void Context::SetCursor(CXCursor cursor)
{
    Cursor = cursor;
}

int Context::GetDepth()
{
    return Depth;
}

void Context::SetDepth(int depth)
{
    Depth = depth;
}

bool Context::IsCompoundStatement()
{
    return CompoundStatement;
}

void Context::SetCompoundStatement(bool compoundStatement)
{
    CompoundStatement = compoundStatement;
}

Context* Context::GetParent()
{
    return Parent;
}

void Context::SetParent(Context* parent)
{
    Parent = parent;
}

vector<Context*>* Context::GetChildren()
{
    return Children;
}

int Context::FindChild(string name)
{
    return Context::FindContext(Children, name);
}

int Context::FindChild(string name, int depth)
{
    return Context::FindContext(Children, name, depth);
}

Context* Context::GetChild(string name)
{
    return Context::GetContext(Children, name);
}

Context* Context::GetChild(int index)
{
    return Context::GetContext(Children, index);
}

Context* Context::RemoveChild(string name)
{
    return Context::RemoveContext(Children, name);
}

Context* Context::RemoveChild(int index)
{
    return Context::RemoveChild(index);
}

Context* Context::PopChild()
{
    return Context::PopContext(Children);
}

void Context::AddChild(string name, bool compoundStatement, CXCursor cursor, Context* parent, int depth)
{
    Context::AddContext(Children, name, compoundStatement, cursor, parent, depth);
}

/**
Adds the context with the specified name, depth compoundStatement e parent properties to the contexts vector.
*/
void Context::AddContext(vector<Context*>* contexts, string name, bool compoundStatement, CXCursor cursor, Context* parent, int depth)
{
    Context* context = new Context(name);
    context->SetDepth(depth);
    context->SetCompoundStatement(compoundStatement);
    context->SetCursor(cursor);
    context->SetParent(parent);
    contexts->push_back(context);
}

int Context::FindContext(vector<Context*>* contexts, CXCursor cursor)
{
    for(int i = 0; i < contexts->size(); i++)
    {
        if(clang_equalCursors((*contexts)[i]->GetCursor(), cursor) == 1)
        {
            return i;
        }
    }
    return -1;
}

int Context::FindContext(vector<Context*>* contexts, string name)
{
    for(int i = 0; i < contexts->size(); i++)
    {
        if((*contexts)[i]->GetName().compare(name))
        {
            return i;
        }
    }
    return -1;
}

int Context::FindContext(vector<Context*>* contexts, string name, int depth)
{
    for(int i = 0; i < contexts->size(); i++)
    {
        if((*contexts)[i]->GetName().compare(name) && (*contexts)[i]->GetDepth() == depth)
        {
            return i;
        }
    }
    return -1;
}

Context* Context::GetContext(vector<Context*>* contexts, CXCursor cursor)
{
    int index = Context::FindContext(contexts, cursor);
    if(index >= 0)
    {
        return (*contexts)[index];
    }
    else
    {
        return NULL;
    }
}

Context* Context::GetContext(vector<Context*>* contexts, string name)
{
    int index = Context::FindContext(contexts, name);
    if(index >= 0)
    {
        return (*contexts)[index];
    }
    else
    {
        return NULL;
    }
}

Context* Context::GetContext(vector<Context*>* contexts, int index)
{
    return (*contexts)[index];
}

Context* Context::GetLastContext(vector<Context*>* contexts)
{
    if(contexts->size() > 0)
    {
        return (*contexts)[contexts->size() - 1];
    }
    else
    {
        return NULL;
    }
}

Context* Context::RemoveContext(vector<Context*>* contexts, string name)
{
    int index = Context::FindContext(contexts, name);
    if(index >= 0)
    {
        return Context::RemoveContext(contexts, index);
    }
    else
    {
        return NULL;
    }
}

Context* Context::RemoveContext(vector<Context*>* contexts, int index)
{
    Context* context = (*contexts)[index];
    contexts->erase(contexts->begin() + index);
    return context;
}

Context* Context::PopContext(vector<Context*>* contexts)
{
    Context* context = contexts->back();
    contexts->pop_back();
    return context;
}

void Context::CheckAddContext(NodeInfo* info, vector<Context*>* contexts)
{
    if(info->GetCursorKind() == CXCursor_FunctionDecl)
    {
        Context::AddContext(contexts, info->GetName(), false, info->GetCursor(), Context::GetLastContext(contexts), info->GetCursorDepth());
    }
    else if(info->GetCursorKind() == CXCursor_ForStmt)
    {
        Context::AddContext(contexts, "for", false, info->GetCursor(), Context::GetLastContext(contexts), info->GetCursorDepth());
    }
    else if(info->GetCursorKind() == CXCursor_IfStmt)
    {
        Context::AddContext(contexts, "if", false, info->GetCursor(), Context::GetLastContext(contexts), info->GetCursorDepth());
    }
    else if(info->GetCursorKind() == CXCursor_WhileStmt)
    {
        Context::AddContext(contexts, "do", false, info->GetCursor(), Context::GetLastContext(contexts), info->GetCursorDepth());
    }
    else if(info->GetCursorKind() == CXCursor_DoStmt)
    {
        Context::AddContext(contexts, "while", false, info->GetCursor(), Context::GetLastContext(contexts), info->GetCursorDepth());
    }

    Context* context = Context::GetContext(contexts, info->GetCursor());

    if(context != NULL)
    {
        cout << "Added context '" << context->GetName() << "' (parent: " << (context->GetParent() == NULL ? " NULL" : context->GetParent()->GetName()) << ")." << endl;
    }
}

void Context::CheckRemoveContext(NodeInfo* info, vector<Context*>* contexts)
{
    if(info->GetCursorDepth() <= Context::GetLastContext(contexts)->GetDepth() && clang_equalCursors(info->GetCursor(), Context::GetLastContext(contexts)->GetCursor()) == 0)
    {
        cout << "Removed context " << Context::GetLastContext(contexts)->GetName() << endl;
        Context::PopContext(contexts);
    }
}

bool Context::IsLoop()
{
    return LoopCondition == NULL;
}

void Context::SetLoopCondition(Operation* op)
{
    LoopCondition = op;
}

Operation* Context::GetLoopCondition()
{
    return LoopCondition;
}

void Context::AddStartValue(Component* comp, int value)
{
    if(GetStartValue(comp) == NULL)
    {
        StartValues->push_back(new pair<Component*,int>(comp, value));
    }
}

pair<Component*,int>* Context::GetStartValue(Component* comp)
{
    for(pair<Component*,int>* p : (*StartValues))
    {
        if(p->first->GetInstanceName().compare(comp->GetInstanceName()) == 0)
        {
            return p;
        }
    }
    return NULL;
}

vector<pair<Component*,int>*>* Context::GetStartValues()
{
    return StartValues;
}

void Context::AddIncrementRate(Component* comp, int rate)
{
    if(GetIncrementRate(comp) == NULL)
    {
        IncrementRates->push_back(new pair<Component*,int>(comp, rate));
    }
}

pair<Component*,int>* Context::GetIncrementRate(Component* comp)
{
    for(pair<Component*,int>* p : (*IncrementRates))
    {
        if(p->first->GetInstanceName().compare(comp->GetInstanceName()) == 0)
        {
            return p;
        }
    }
    return NULL;
}

vector<pair<Component*,int>*>* Context::GetIncrementRates()
{
    return IncrementRates;
}

int Context::GetLoopClockCount()
{
    int maxLoopCount = 0;
    for(pair<Component*,int>* p : (*GetStartValues()))
    {
        int startValue = p->second;
        pair<Component*,int>* incrementComponent = GetIncrementRate(p->first);

        if(incrementComponent != NULL && incrementComponent->first != NULL)
        {
            int incrementRate = incrementComponent->second;
            int maxValue = 0;
            int currentLoopCount = 0;

            Component* first = (*GetLoopCondition()->GetDependencies())[0]->first;
            Component* second = (*GetLoopCondition()->GetDependencies())[1]->first;

            if(p->first->GetInstanceName().compare(first->GetInstanceName()) == 0)
            {
                if(second->IsIoComponent())
                {
                    maxValue = Component::GetIntegerValue(((IOComponent*) second)->GetValue());
                }
                else
                {
                    IOComponent* secondParent = (IOComponent*) second->GetPort("I0")->GetPortSignals()[0]->GetSendingPort("O0")->GetParentComponent();
                    maxValue = Component::GetIntegerValue(secondParent->GetValue());
                }
            }

            if(maxValue > startValue)
            {
                currentLoopCount = maxValue - startValue / incrementRate;
            }

            if(currentLoopCount > maxLoopCount)
            {
                maxLoopCount = currentLoopCount;
            }
        }
    }

    return maxLoopCount;
}

int Context::GetContextTreeClycleCount()
{
    int cycles = GetCycleClockCount();

    Context* parent = GetParent();
    while(parent != NULL)
    {
        cycles += parent->GetCycleClockCount();
        parent = parent->GetParent();
    }

    return cycles;
}

void Context::PrintContext(vector<Context*>* contexts)
{
    cout << "Printing contexts (size = " << contexts->size() << ")" << endl;
    for(int i = 0; i < contexts->size(); i++)
    {
        Context* c = (*contexts)[i];
        CXString name = clang_getCursorSpelling(c->GetCursor());
        string tokenName = clang_getCString(name);
        CXCursorKind kind = clang_getCursorKind(c->GetCursor());

        cout << "context name: " << c->GetName() << endl;

        Context* parent = c->GetParent();
        if(parent != NULL)
        {
            cout << c->GetName() << "(depth = " << c->GetDepth() << ")" << ": IsCompound = " << c->IsCompoundStatement() << ", parentName = " << c->GetParent()->GetName() << ", ParentCursor = '" << tokenName << "' (" << kind << ")" << endl;
        }
        else
        {
            cout << c->GetName() << "(depth = " << c->GetDepth() << ")" << ": IsCompound = " << c->IsCompoundStatement() << ", parentName = NULL, ParentCursor = '" << tokenName << "' (" << kind << ")" << endl;
        }
    }
    cout << "End of contexts printing" << endl;
}

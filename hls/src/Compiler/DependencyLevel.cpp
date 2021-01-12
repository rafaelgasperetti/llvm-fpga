#include<iostream>
#include<string>
#include<vector>

#include<Compiler/DependencyLevel.hpp>

int DependencyLevel::TYPE_READ = 0;
int DependencyLevel::TYPE_WRITE = 1;

DependencyLevel::DependencyLevel(int level, int type)
{
    SetLevel(level);
    SetType(type);
}

void DependencyLevel::SetLevel(int level)
{
    Level = level >= 0 ? level : 0;
}

int DependencyLevel::GetLevel()
{
    return Level;
}

void DependencyLevel::AddDependency(Component* comp)
{
    if(comp != NULL)
    {
        if(!Contains(comp))
        {
            if(GetType() == TYPE_WRITE)
            {
                Dependencies.push_back(comp);
                comp->AddDependencyLevel(this);
            }
            else
            {
                throw invalid_argument("Only write type dependency levels may have component dependencies directly, otherwise, just throught operations.");
            }
        }
        else
        {
            throw invalid_argument("A dependency level must never contains twice the same component. Problem found at component '" + comp->GetInstanceName() + "'");
        }
    }
    else
    {
        throw invalid_argument("A dependency level must never contains a NULL component.");
    }
}

void DependencyLevel::RemoveDependency(string name)
{
    int idx = IndexOf(name);
    if(idx >= 0)
    {
        Component* comp = GetDependency(name);
        comp->RemoveDependencyLevel(GetLevel(), GetType());
        Dependencies.erase(Dependencies.begin() + idx);
    }
}

Component* DependencyLevel::GetDependency(string name)
{
    int idx = IndexOf(name);
    if(idx >= 0)
    {
        return Dependencies[idx];
    }
    else
    {
        return NULL;
    }
}

vector<Component*> DependencyLevel::GetDependencies()
{
    return Dependencies;
}

bool DependencyLevel::Contains(Component* comp)
{
    return IndexOf(comp) >= 0;
}

bool DependencyLevel::Contains(string name)
{
    return IndexOf(name) >= 0;
}

int DependencyLevel::IndexOf(Component* comp)
{
    for(int i = 0; i < Dependencies.size(); i++)
    {
        if(comp->GetInstanceName().compare(Dependencies[i]->GetInstanceName()) == 0)
        {
            return i;
        }
    }
    return -1;
}

int DependencyLevel::IndexOf(string name)
{
    for(int i = 0; i < Dependencies.size(); i++)
    {
        if(name.compare(Dependencies[i]->GetInstanceName()) == 0)
        {
            return i;
        }
    }
    return -1;
}

void DependencyLevel::MoveDependency(DependencyLevel* from,DependencyLevel* to, string name)
{
    Component* mov = from->GetDependency(name);
    if(mov != NULL)
    {
        from->RemoveDependency(name);
        to->AddDependency(mov);
        mov->AddDependencyLevel(to);

        cout << "Moved " << mov->GetInstanceName() << " from level " << from->GetLevel()
            << " to level " << to->GetLevel() << endl;
    }
    else
    {
        throw invalid_argument("It is not possible to move a NULL component (" + name + ") from dependency level '" + to_string(from->GetLevel()) + "' to dependency level '" + to_string(to->GetLevel()) + "'.");
    }
}

void DependencyLevel::ManageDependencyLevels(DependencyLevel* to, Operation* op, vector<Operation*>* vec)//Not needed.
{
    /*cout << to << ", " << op << ", " << vec << endl;
    for(pair<Component*,int>* dep : (*op->GetDependencies()))
    {
        if(op->GetDependencyLevel() != NULL && op->GetDependencyLevel()->GetDependency(dep->first->GetInstanceName()) != NULL && to->GetDependency(dep->first->GetInstanceName()) == NULL)
        {
            DependencyLevel::MoveDependency(op->GetDependencyLevel(), to, dep->first->GetInstanceName());
            cout << "Moved dependency " << dep->first->GetInstanceName() << " from level " << op->GetDependencyLevel()->GetLevel() << " to " << to->GetLevel() << " on " << op->GetName() << endl;
        }
        else if(to->GetDependency(dep->first->GetInstanceName()) == NULL)
        {
            to->AddDependency(dep->first);
            cout << "Added dependency " << dep->first->GetInstanceName() << " to level " << to->GetLevel() << " on " << op->GetName() << endl;
        }
    }

    DependencyLevel* old = op->GetDependencyLevel();
    op->SetDependencyLevel(to);
    cout << "Moved " << op->GetName() << " from dependency level " << (old == NULL ? -1 : old->GetLevel()) << " to " << op->GetDependencyLevel()->GetLevel() << endl;*/
}

void DependencyLevel::ReplicateDependency(DependencyLevel* from,DependencyLevel* to, string name)
{
    Component* mov = from->GetDependency(name);
    if(mov != NULL)
    {
        to->AddDependency(mov);
        mov->AddDependencyLevel(to);

        cout << "Moved " << mov->GetInstanceName() << " from level " << from->GetLevel()
            << " to level " << to->GetLevel() << endl;
    }
    else
    {
        throw invalid_argument("It is not possible to move a NULL component from dependency level '" + to_string(from->GetLevel()) + "' to dependency level '" + to_string(to->GetLevel()) + "'.");
    }
}

void DependencyLevel::SetType(int type)
{
    if(type == DependencyLevel::TYPE_READ || type == DependencyLevel::TYPE_WRITE)
    {
        Type = type;
    }
    else
    {
        throw invalid_argument("The dependency level type '" + to_string(type) + "' is not valid. Valid types are read (" + to_string(DependencyLevel::TYPE_READ) + ") or write (" + to_string(DependencyLevel::TYPE_WRITE) + ").");
    }
}

int DependencyLevel::GetType()
{
    return Type;
}
void DependencyLevel::SetCycleNumbers(int numbers)
{
    if(numbers < 1)
    {
        CycleNumbers = numbers;
    }
    else
    {
        CycleNumbers = 1;
    }
}

int DependencyLevel::GetCycleNumbers()
{
    return CycleNumbers;
}

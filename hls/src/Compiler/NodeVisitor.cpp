#include<string.h>

#include<Compiler/NodeVisitor.hpp>

#include<Tools/MessageLibrary.hpp>

#include<Components/cmp_block_ram.hpp>
#include<Components/cmp_reg_op.hpp>
#include<Components/cmp_if_lt_op_s.hpp>
#include<Components/cmp_mult_op_s.hpp>
#include<Components/cmp_counter.hpp>
#include<Components/cmp_add_op_s.hpp>
#include<Components/cmp_sub_op_s.hpp>
#include<Components/cmp_mult_op_s.hpp>
#include<Components/cmp_div_op_s.hpp>

using namespace std;

NodeVisitor* Visitor = new NodeVisitor();

void NodeVisitor::SetDesign(Design* design)
{
    VisitorDesign = design;
}

Design* NodeVisitor::GetDesign()
{
    return VisitorDesign;
}

void NodeVisitor::SetTranslationUnit(CXTranslationUnit translationUnit)
{
    TranslationUnit = translationUnit;
}

CXTranslationUnit NodeVisitor::GetTranslationUnit()
{
    return TranslationUnit;
}

void NodeVisitor::AddVariable(string name, Component* relatedComponent, CXCursor relatedCursor)
{
    Variable* v = new Variable(name);
    v->SetRelatedComponent(relatedComponent);
    v->SetRelatedCursor(relatedCursor);
    GetVariables()->push_back(v);
}

vector<Context*>* NodeVisitor::GetContexts()
{
    return Contexts;
}

vector<Variable*>* NodeVisitor::GetVariables()
{
    return Variables;
}

vector<NodeInfo*>* NodeVisitor::GetInfoHistory()
{
    return InfoHistory;
}

Variable* NodeVisitor::GetVariable(string name)
{
    for(Variable* v : (*Visitor->GetVariables()))
    {
        if(name.compare(v->GetName()) == 0)
        {
            return v;
        }
    }
    return NULL;
}

Variable* NodeVisitor::GetVariable(CXCursor cursor)
{
    for(Variable* v : (*Visitor->GetVariables()))
    {
        if(clang_equalCursors(cursor, v->GetRelatedCursor()) == 1)
        {
            return v;
        }
    }
    return NULL;
}

CXChildVisitResult NodeVisitor::ChooseVisitor(NodeInfo* info)
{
    CXChildVisitResult result;

	NodeInfo::PrintCursorInfo("Choosing for => ", info);

    switch(info->GetCursorKind())
    {
        case CXCursor_DeclStmt:
            result = VisitDeclaration(info);
            break;
        case CXCursor_VarDecl:
            result = VisitVariable(info);
            break;
        case CXCursor_ForStmt:
            result = VisitFor(info);
            break;
        case CXCursor_WhileStmt:
            result = CXChildVisit_Recurse;
            break;
        case CXCursor_DoStmt:
            result = CXChildVisit_Recurse;
            break;
        case CXCursor_ClassDecl:
            result = CXChildVisit_Recurse;
            break;
        case CXCursor_FunctionDecl:
            result = VisitFunction(info);//Only main supported for now
            break;
        case CXCursor_ObjCInstanceMethodDecl:
            result = CXChildVisit_Recurse;
            break;
        case CXCursor_IntegerLiteral:
            result = VisitIntLiteral(info);
            break;
        case CXCursor_DeclRefExpr:
            result = VisitDeclarationReference(info);
            break;
        case CXCursor_FirstExpr://Same number as "CXCursor_UnexposedExpr"
            result = VisitFirstExpression(info);
            break;
        case CXCursor_UnaryOperator:
            result = CXChildVisit_Recurse;
            break;
        case CXCursor_BinaryOperator:
            result = VisitBinaryOperator(info);
            break;
        case CXCursor_ReturnStmt:
            result = VisitReturn(info);
            break;
        case CXCursor_CompoundStmt:
            result = VisitCompoundStatement(info);
            break;
        case CXCursor_IfStmt:
            result = VisitIf(info);
            break;
        case CXCursor_CompoundAssignOperator:
            result = VisitCompoundAssignOperator(info);
            break;
        case CXCursor_ArraySubscriptExpr:
            result = VisitArraySubscript(info);
            break;
        default:
            NodeInfo::PrintCursorInfo("UNKNOWN TOKEN KIND OF => ", info);
            NodeInfo::PrintCursorTokens(info->GetCursor());
            result = CXChildVisit_Recurse;
            break;
    }

    return result;
}

CXChildVisitResult NodeVisitor::VisitDeclaration(NodeInfo* info)
{
    NodeInfo::PrintCursorInfo("Visiting declaration => ", info);

    return CXChildVisit_Recurse;
}

CXChildVisitResult NodeVisitor::VisitVariable(NodeInfo* info)
{
    NodeInfo::PrintCursorInfo("Visiting variable => ", info);
    Variable* var = Visitor->GetVariable(info->GetCursor());

    Component* comp = var->GetRelatedComponent();

    Visitor->GetDesign()->AddDeclaredName(comp->GetInstanceName());
    Visitor->GetDesign()->AddComponent(comp, info->GetCursor());
    Visitor->GetDesign()->AddToNumberOfComponents(comp->GetComponentName());
    comp->ConnectDefaultSignals(Visitor->GetDesign());

    int intVal = 0;

    if(comp->IsBlockRam())
    {
        string intSize =  NodeInfo::GetTokenFromCursor(info->GetCursor(), -3);

        if(intSize.compare("]") != 0)
        {
            Component* intSizeComponent = Visitor->GetDesign()->GetComponent("\\" + intSize + "\\").IdentifiedComponent;

            if(intSizeComponent != NULL && intSizeComponent->GetComponentName().compare("const_op") == 0)
            {
                intVal = Component::GetIntegerValue(((Cmp_const_op*) intSizeComponent)->GetValue());
                ((Cmp_block_ram*) comp)->SetMaxSize(intVal);
            }
        }
    }
    else
    {
        comp->ConnectDefaultSignals(Visitor->GetDesign());
    }

    Visitor->GetDesign()->AddVariableName("\\" + var->GetName() + "\\");

    cout << "Added " << comp->GetInstanceName() << " (" << comp->GetComponentName() << (comp->IsBlockRam() ? ", maxSize: " + to_string(((Cmp_block_ram*) comp)->GetMaxSize()) : "") << ")" << endl;

    NodeInfo* grandParend = info->GetCursorHistory(NodeInfo::History_2_Levels_Above);

    if(info->GetParentCursorKind() == CXCursor_DeclStmt && grandParend->GetCursorKind() == CXCursor_ForStmt)
    {
        Context* context = Context::GetContext(Visitor->GetContexts(), grandParend->GetCursor());
        context->AddStartValue(comp, StringUtils::StringToInt(NodeInfo::GetTokenFromCursor(info->GetCursor(), -2)));
        cout << "Added start value " << NodeInfo::GetTokenFromCursor(info->GetCursor(), -2) << " for " << comp->GetInstanceName() << endl;
    }

    return CXChildVisit_Recurse;
}

CXChildVisitResult NodeVisitor::VisitIntCounter(NodeInfo* info)
{
    //Unused 17/04
    NodeInfo::PrintCursorInfo("Visiting int counter => ", info);
	return CXChildVisit_Recurse;
}

CXChildVisitResult NodeVisitor::VisitDeclarationReference(NodeInfo* info)
{
    NodeInfo::PrintCursorInfo("Visiting DeclarationReference => ", info);
    NodeInfo::PrintCursorTokens(info->GetCursor());

    CXChildVisitResult result;

    NodeInfo* grandParent = info->GetCursorHistory(NodeInfo::History_2_Levels_Above);//Grandparent (parent of parent is always 0 on this parameter)

    if(info->GetParentCursorKind() == CXCursor_FirstExpr)
    {
        cout << "Visiting declaration reference after first expression" << endl;

        string firstCursorToken = NodeInfo::GetTokenFromCursor(info->GetCursor(), 0);
        string firstGrandParantToken = NodeInfo::GetTokenFromCursor(grandParent->GetCursor(), 0);

        if(grandParent->GetCursorKind() == CXCursor_ArraySubscriptExpr && firstCursorToken.compare(firstGrandParantToken) != 0)
        {
            NodeInfo::PrintCursorInfo("Grandparent is array subscript => ", grandParent);
            NodeInfo::PrintCursorTokens(info->GetCursor());
            NodeInfo::PrintCursorTokens(grandParent->GetCursor());

            Component* index = Visitor->GetDesign()->GetComponent("\\" + firstCursorToken + "\\").IdentifiedComponent;
            Component* memory = Visitor->GetDesign()->GetComponent("\\" + firstGrandParantToken + "\\").IdentifiedComponent;

            cout << "Index: " << index->GetInstanceName() << ", Memory: " << memory->GetInstanceName() << endl;

            string addressSignalName = StringUtils::Replace(index->GetInstanceName(), "\\", "") + "_address_to_" + StringUtils::Replace(memory->GetInstanceName(), "\\", "");
            Signal* ramAddress = new Signal("\\" + addressSignalName + "\\", "std_logic_vector", memory->GetPort("address")->GetWidth());
            Visitor->GetDesign()->AddNamedSignal(ramAddress, addressSignalName);
            Visitor->GetDesign()->ConnectPorts(index, index->GetPort("output"), memory, memory->GetPort("address"), ramAddress);
            cout << "Connected '" << index->GetInstanceName() << "' (port '" << index->GetPort("output")->GetName() << "') to " << memory->GetInstanceName() << "' (port '" << memory->GetPort("address")->GetName() << "')" << endl;
        }
        result = CXChildVisit_Continue;
    }
    else if(info->GetParentCursorKind() == CXCursor_UnaryOperator)
    {
        cout << "Visiting unary operation" << endl;
        VisitUnaryOperator(info);
        result = CXChildVisit_Continue;
    }
    else if(info->GetParentCursorKind() == CXCursor_BinaryOperator)//Not using right now
    {
        cout << "Declaration reference after binary operator" << endl;
        //When a declaration is from array and uses instantiation. Dotprod doesnt have it, another example may do.
        /*string binOp = NodeInfo::GetTokenFromCursor(info->GetCursor(), -1);
        cout << "Binary operation: '" << binOp << "'" << endl;
        VisitFirstBinaryOperator(info, binOp);
        Operation* operation = Visitor->GetOperation(info->GetParentCursor());

        if(operation->GetName().compare(NodeInfo::Operation_Attribution) != 0)
        {
            Component* opComp = operation->GetRelatedComponent();
        }*/

        result = CXChildVisit_Continue;
    }
    else if(NodeInfo::IsArray(info->GetParentCursorType()))
    {
        Cmp_block_ram* ram = (Cmp_block_ram*) Visitor->GetDesign()->GetComponent("\\" + info->GetParentName() + "\\").IdentifiedComponent;
        Component* ramSize = Visitor->GetDesign()->GetComponent("\\" + info->GetName() + "\\").IdentifiedComponent;

        if(ramSize->GetComponentName().compare("reg_op") == 0)
        {
            Signal* ramSizeSignal = ((Cmp_reg_op*) ramSize)->GetPort("I0")->GetPortSignal();

            Component* parentComponent = ramSizeSignal->GetSendingPort("O0")->GetParentComponent();

            cout << "Parent component " << parentComponent->GetInstanceName() << " from " << ramSize->GetInstanceName() << endl;

            //Supposing that the O0 port parent component is a Cmp_const_op
            int intVal = Component::GetIntegerValue(((Cmp_const_op*) parentComponent)->GetValue());
            ram->SetMaxSize(intVal);
        }
        cout << "Component: " << ramSize->GetInstanceName() << " (" << ramSize->GetComponentName() << ")" << endl;
        //verificar se é uma constante ou uma variável
        result = CXChildVisit_Recurse;
    }
    else
    {
        result = CXChildVisit_Recurse;
    }

	return result;
}

CXChildVisitResult NodeVisitor::VisitFirstExpression(NodeInfo* info)
{
    NodeInfo::PrintCursorInfo("Visiting first expression => ", info);

    if(info->GetParentCursorKind() == CXCursor_VarDecl)
    {
        Component* parent = Visitor->GetDesign()->GetComponent(info->GetParentCursor());
        Component* current = Visitor->GetDesign()->GetComponent("\\" + info->GetName() + "\\").IdentifiedComponent;

        Signal* intermediate = new Signal("\\from_" + StringUtils::Replace(parent->GetInstanceName(),"\\","") + "_to_" + StringUtils::Replace(current->GetInstanceName(),"\\","") + "\\","std_logic_vector",Visitor->GetDesign()->GetWidth());

        DependencyLevel* maxLvl = parent->GetMaxDependencyLevel(DependencyLevel::TYPE_WRITE);

        if(maxLvl->GetLevel() > 0)
        {
            parent->SetStartClock(maxLvl->GetLevel() - 1);
            parent->SetCycleClockCount(maxLvl->GetCycleNumbers());
            Visitor->GetDesign()->ConnectDelayedPorts(current, current->GetPort("I0"), parent, parent->GetPort("O0"), intermediate, maxLvl->GetLevel() + parent->GetContext()->GetStartClock());
            cout << "Connected from " << current->GetInstanceName() << " to " << parent->GetInstanceName() << " with delay of " << maxLvl->GetLevel() << endl;
        }
        else
        {
            Visitor->GetDesign()->ConnectPorts(current, current->GetPort("I0"), parent, parent->GetPort("O0"), intermediate);
            cout << "Connected from " << current->GetInstanceName() << " to " << parent->GetInstanceName() << endl;
        }
    }
    else if(info->GetParentCursorKind() == CXCursor_ReturnStmt)
    {
        cout << "Parent = return, connecting '" << info->GetName() << "' to the output signal" << endl;
        Signal* out = Visitor->GetDesign()->GetSignal("result");
        Component* outComp = Visitor->GetDesign()->GetComponent("\\" + info->GetName() + "\\").IdentifiedComponent;
        cout << "Added signal '" + out->GetSignalName() + "'" << endl;
        Visitor->GetDesign()->ConnectPortSignal(outComp, outComp->GetPort("O0"), out, true);
        cout << "Connected '" << outComp->GetInstanceName() << "' O0 port to '" << out->GetSignalName() << "' signal" << endl;
    }
    else if(info->GetParentCursorKind() == CXCursor_BinaryOperator && Visitor->GetOperation(info->GetParentCursor())->GetName().compare(NodeInfo::Operation_Attribution) != 0)
    {
        Operation* operation = Visitor->GetOperation(info->GetParentCursor());
        cout << "Parent operation: " << operation->GetName() << endl;

        Component* componentOperation = operation->GetRelatedComponent();
        Component* operand;
        Signal* operandToOperator;
        string componentOperationPort;

        Component* parentOperation = NULL;
        NodeInfo* grandParent = info->GetCursorHistory(NodeInfo::History_2_Levels_Above);//Grandparent (parent of parent is always 0 on this parameter)
        NodeInfo::PrintCursorInfo("Operation: " + operation->GetName() + ", GrandParent => ", grandParent);

        if(NodeInfo::GetTokenFromCursor(info->GetCursor(), -1).compare(";") == 0)
        {
            if((*operation->GetDependencies())[1]->second >= 0)
            {
                operand = (*operation->GetDependencies())[2]->first;
            }
            else
            {
                operand = (*operation->GetDependencies())[1]->first;
            }
            componentOperationPort = "I1";
        }
        else
        {
            operand = (*operation->GetDependencies())[0]->first;
            componentOperationPort = "I0";

            parentOperation = Visitor->GetDesign()->GetComponent(grandParent->GetCursor());
        }

        cout << "Found component '" << operand->GetInstanceName() << "' (" << operand->GetComponentName() << ") to operate '" << operation->GetName() << "'" << endl;

        operand->ConnectDefaultSignals(Visitor->GetDesign());

        string operandPortName;

        if(operand->GetComponentName().compare("counter") == 0)
        {
            operandPortName = "output";
            operand->GetMap("condition")->SetValue(to_string(Cmp_counter::GetConditionValue(operation->GetName())));
        }
        else if(operand->GetComponentName().compare("reg_op") == 0)
        {
            operandPortName = "O0";
        }
        else if(operand->GetComponentName().compare("const_op") == 0)
        {
            operandPortName = "O0";
        }
        else if(operand->GetComponentName().compare("block_ram") == 0)
        {
            operandPortName = "data_out";
        }
        else
        {
            throw invalid_argument("Unexpected component name '" + operand->GetComponentName() + " at VisitFirstExpression from parent = 'CXCursor_BinaryOperator'.");
        }

        string sigName;

        sigName = "from_" + StringUtils::Replace(operand->GetInstanceName(), "\\", "") + "_to_" + StringUtils::Replace(componentOperation->GetInstanceName(), "\\", "");
        operandToOperator = new Signal("\\" + sigName + "\\", "std_logic_vector", Visitor->GetDesign()->GetWidth());
        Visitor->GetDesign()->AddNamedSignal(operandToOperator, sigName);
        Visitor->GetDesign()->ConnectPorts(operand, operand->GetPort(operandPortName), componentOperation, componentOperation->GetPort(componentOperationPort), operandToOperator);
        cout << "Created signal '" << operandToOperator->GetSignalName() << "' and connected from '"
            << operand->GetInstanceName() << "' (" << operand->GetComponentName() << ") port '" << operand->GetPort(operandPortName)->GetName() << "' to '"
            << componentOperation->GetInstanceName() << "' (" << componentOperation->GetComponentName() << ") port '" << componentOperation->GetPort(componentOperationPort)->GetName() << "'." << endl;

        if(parentOperation != NULL)//means that this operation does not have an operation 2 levels above (this operation's output must connect the above's input
        {
            cout << "Parent operator '" << parentOperation->GetInstanceName() << "' (" << parentOperation->GetComponentName() << ")" << endl;

            sigName = "from_" + StringUtils::Replace(componentOperation->GetInstanceName(), "\\", "") + "_to_" + StringUtils::Replace(parentOperation->GetInstanceName(), "\\", "");
            Signal* operationToParentOperation = new Signal("\\" + sigName + "\\", "std_logic_vector", Visitor->GetDesign()->GetWidth());
            Visitor->GetDesign()->AddNamedSignal(operationToParentOperation, sigName);

            //the connection depends on the parent operation type
            if(grandParent->GetCursorKind() == CXCursor_CompoundAssignOperator)//if the parent operation is compound assignment
            {
                Visitor->GetDesign()->ConnectPorts(componentOperation, componentOperation->GetPort("O0"), parentOperation, parentOperation->GetPort("I1"), operationToParentOperation);
                cout << "Created signal '" << operationToParentOperation->GetSignalName() << "' and connected from '"
                    << componentOperation->GetInstanceName() << "' (" << componentOperation->GetComponentName() << ") port '" << componentOperation->GetPort("O0")->GetName() << "' to '"
                    << parentOperation->GetInstanceName() << "' (" << parentOperation->GetComponentName() << ") port '" << parentOperation->GetPort("I1")->GetName() << "'." << endl;
            }
        }
        else if(grandParent->GetCursorKind() == CXCursor_ForStmt)
        {
            cout << "GrandParent is 'for' cursor kind" << endl;

            Component* lastOperator = NULL;

            if(operation->GetName().compare(";") != 0)
            {
                lastOperator = Visitor->GetDesign()->GetComponent("\\" + NodeInfo::GetTokenFromCursor(info->GetParentCursor(), -2) + "\\").IdentifiedComponent;
                cout << "lastOperator: " << lastOperator->GetInstanceName() << " (" << lastOperator->GetComponentName() << ")" << endl;

                if(lastOperator->GetComponentName().compare("const_op") == 0)
                {
                    Signal* lastOperatorSignal = ((Cmp_const_op*) lastOperator)->GetRelatedSignal();
                    /*string lastOpName = "from_" + StringUtils::Replace(lastOperator->GetInstanceName(), "\\", "") + "_to_" + StringUtils::Replace(componentOperation->GetInstanceName(), "\\", "");
                    operandToOperator = new Signal("\\" + lastOpName + "\\", "std_logic_vector", Visitor->GetDesign()->GetWidth());
                    Visitor->GetDesign()->AddNamedSignal(operandToOperator, lastOpName);
                    Visitor->GetDesign()->ConnectPorts(lastOperator, lastOperator->GetPort(operandPortName), componentOperation, componentOperation->GetPort("I1"), operandToOperator);*/

                    Visitor->GetDesign()->ConnectPortSignal(componentOperation, componentOperation->GetPort("I1"), lastOperatorSignal, false);

                    cout << "Connected const signal '" << lastOperatorSignal->GetSignalName() << "' to '"
                         << componentOperation->GetInstanceName() << "' (" << componentOperation->GetComponentName() << ") port '" << componentOperation->GetPort("I1")->GetName() << "'." << endl;
                }
                /*else if(lastOperator->GetComponentName().compare("xxxx") == 0/Others components
                {
                    string lastOpName = "from_" + StringUtils::Replace(lastOperator->GetInstanceName(), "\\", "") + "_to_" + StringUtils::Replace(componentOperation->GetInstanceName(), "\\", "");
                    operandToOperator = new Signal("\\" + lastOpName + "\\", "std_logic_vector", Visitor->GetDesign()->GetWidth());
                    Visitor->GetDesign()->AddNamedSignal(operandToOperator, lastOpName);
                    Visitor->GetDesign()->ConnectPorts(lastOperator, lastOperator->GetPort(operandPortName), componentOperation, componentOperation->GetPort("I1"), operandToOperator);
                }*/
                else
                {
                    lastOperator = NULL;
                }
            }

            if(operation->GetName().compare(";") == 0 || (operation->GetName().compare(";") != 0 && lastOperator != NULL))
            {
                vector<Port*> sendingPortsI0 = componentOperation->GetPort("I0")->GetPortSignal()->GetSendingPorts();
                vector<Port*> sendingPortsI1 = componentOperation->GetPort("I1")->GetPortSignal()->GetSendingPorts();

                cout << "Identifying counter's break condition, sending ports: I0 = " << sendingPortsI0.size() << ", I1 = " << sendingPortsI1.size() << endl;

                Component* firstOperand = NULL;
                string firstOperandInputPort;

                Component* secondOperand = NULL;
                string secondOperandOutputPort;

                if(sendingPortsI0.size() == 1 && sendingPortsI0[0]->GetParentComponent() != NULL)//first operand is a counter
                {
                    if(sendingPortsI0[0]->GetParentComponent()->GetComponentName().compare("counter") == 0)
                    {
                        firstOperand = sendingPortsI0[0]->GetParentComponent();
                        firstOperandInputPort = "termination";
                    }
                }

                if(sendingPortsI1.size() == 1 && sendingPortsI1[0]->GetParentComponent() != NULL)//first operand is a counter
                {
                    if(sendingPortsI1[0]->GetParentComponent()->GetComponentName().compare("reg_op") == 0)
                    {
                        secondOperand = sendingPortsI1[0]->GetParentComponent();
                        secondOperandOutputPort = "O0";
                    }
                    else if(sendingPortsI1[0]->GetParentComponent()->GetComponentName().compare("const_op") == 0)
                    {
                        secondOperand = sendingPortsI1[0]->GetParentComponent();
                        secondOperandOutputPort = "O0";
                    }
                }
                else if(lastOperator->GetComponentName().compare("const_op") == 0)
                {
                    secondOperand = lastOperator;
                    secondOperandOutputPort = "O0";
                }

                if(firstOperand != NULL && secondOperand != NULL)
                {
                    cout << "first and second operands are not null" << endl;

                    if(secondOperand->GetComponentName().compare("const_op") == 0)
                    {
                        Visitor->GetDesign()->ConnectPortSignal(firstOperand, firstOperand->GetPort(firstOperandInputPort), ((Cmp_const_op*) secondOperand)->GetRelatedSignal(), false);

                        cout << "Connected const signal '" << ((Cmp_const_op*) secondOperand)->GetRelatedSignal()->GetSignalName() << "' to '"
                             << firstOperand->GetInstanceName() << "' (" << firstOperand->GetComponentName() << ") port '" << firstOperand->GetPort("I1")->GetName() << "'." << endl;
                    }
                    else
                    {
                        string cmpSigName = "from_" + StringUtils::Replace(secondOperand->GetInstanceName(), "\\", "") + "_to_" + StringUtils::Replace(firstOperand->GetInstanceName(), "\\", "");
                        Signal* cmpSig = new Signal("\\" + cmpSigName + "\\", "std_logic_vector", Visitor->GetDesign()->GetWidth());
                        Visitor->GetDesign()->AddNamedSignal(cmpSig, cmpSigName);
                        Visitor->GetDesign()->ConnectPorts(secondOperand, secondOperand->GetPort("O0"), firstOperand, firstOperand->GetPort(firstOperandInputPort), cmpSig);
                        cout << "Created signal '" << cmpSig->GetSignalName() << "' and connected from '"
                            << secondOperand->GetInstanceName() << "' (" << secondOperand->GetComponentName() << ") port '" << secondOperand->GetPort("O0")->GetName() << "' to '"
                            << firstOperand->GetInstanceName() << "' (" << firstOperand->GetComponentName() << ") port '" << firstOperand->GetPort(firstOperandInputPort)->GetName() << "'." << endl;
                    }
                }
            }
        }
    }
    else if(info->GetParentCursorKind() == CXCursor_ArraySubscriptExpr)
    {
        cout << "Visiting first expression after array subscript" << endl;
    }

    return CXChildVisit_Recurse;
}

CXChildVisitResult NodeVisitor::VisitUnexposedExpression(NodeInfo* info)
{
    return CXChildVisit_Recurse;
}

void NodeVisitor::VisitUnaryOperator(NodeInfo* info)
{
    Operation* op = Visitor->GetOperation(info->GetParentCursor());

    Component* firstDependency = (*op->GetDependencies())[0]->first;
    IOComponent* parentSecondDependency = NULL;

    Visitor->GetDesign()->IncrementNumberOfOperations();

    cout << "Component at design: " << firstDependency->GetInstanceName() << " (" << firstDependency->GetComponentName() << ") to operate '" << op->GetName() << "'" << endl;

    if(op->GetName().compare(NodeInfo::Operation_CompoundSum) == 0)
    {
        ((Cmp_counter*) firstDependency)->GetMap("increment")->SetValue("1");
        ((Cmp_counter*) firstDependency)->GetMap("down")->SetValue("0");
    }
    else if(op->GetName().compare(NodeInfo::Operation_CompoundSubtraction) == 0)
    {
        ((Cmp_counter*) firstDependency)->GetMap("increment")->SetValue("-1");
        ((Cmp_counter*) firstDependency)->GetMap("down")->SetValue("1");
    }
    else
    {
        throw invalid_argument("Unexpected unary operation '" + op->GetName() + "': " + to_string(MessageLibrary::ERROR_UNEXPECTED_OPERATION));
    }

    Context* context = Context::GetContext(Visitor->GetContexts(), info->GetCursorHistory(NodeInfo::History_2_Levels_Above)->GetCursor());
    cout << "Context: " << context->GetName() << endl;

    Component* firstIfDependency = (*context->GetLoopCondition()->GetDependencies())[0]->first;

    if(op->IsCompound())
    {
        if(op->GetName().compare(NodeInfo::Operation_CompoundSum) == 0)
        {
            context->AddIncrementRate(firstDependency, 1);
            cout << "Added increment rate of 1 to " << firstDependency->GetInstanceName() << endl;
        }
        else
        {
            context->AddIncrementRate(firstDependency, -1);
            cout << "Added increment rate of -1 to " << firstDependency->GetInstanceName() << endl;
        }

        context->SetCycleClockCount(context->GetLoopClockCount());
        cout << "Set context '" << context->GetName() << "' clock cycles to " << context->GetCycleClockCount()
            << " (parent '" << (context->GetParent() != NULL ? context->GetParent()->GetName() + "' is " + to_string(context->GetParent()->GetCycleClockCount()): " NULL") << ")" << endl;
    }

    cout << "Unary operator Added " << firstIfDependency->GetComponentName() << ": " << firstIfDependency->GetInstanceName() << " to operate '" << op->GetName() << "'" << endl;
}

CXChildVisitResult NodeVisitor::VisitBinaryOperator(NodeInfo* info)
{
    NodeInfo::PrintCursorInfo("Visiting binary operator (Array: " + to_string(NodeInfo::IsArray(info->GetParentCursorType())) + ") => ", info);

    Operation* op = Visitor->GetOperation(info->GetCursor());

    if(op->GetName().compare(NodeInfo::Operation_Attribution) != 0)
    {
        Visitor->GetDesign()->AddDeclaredName(op->GetRelatedComponent()->GetInstanceName());
        Visitor->GetDesign()->AddComponent(op->GetRelatedComponent(), info->GetCursor());
        Visitor->GetDesign()->AddToNumberOfComponents(op->GetRelatedComponent()->GetComponentName());

        cout << "Added component " << op->GetRelatedComponent()->GetInstanceName() << " (" << op->GetRelatedComponent()->GetComponentName() << ") for operation " << op->GetName() << endl;
    }

    if(info->GetParentCursorKind() == CXCursor_CompoundAssignOperator)//Maybe there are other places to remove the context too
    {
        cout << "Binary operator after compound assign operator" << endl;
    }
    else if(info->GetParentCursorKind() == CXCursor_ForStmt)
    {
        cout << "Binary operator after for stmt" << endl;

        Operation* op = Visitor->GetOperation(info->GetCursor());
        Context* context = Context::GetContext(Visitor->GetContexts(), info->GetParentCursor());

        Component* firstDependency = (*op->GetDependencies())[0]->first;
        Component* secondDependency = op->GetDependencies()->size() > 1 ? (*op->GetDependencies())[1]->first : NULL;
        IOComponent* parentSecondDependency = NULL;

        if(secondDependency != NULL && !secondDependency->IsIoComponent())
        {
            parentSecondDependency = (IOComponent*) secondDependency->GetPort("I0")->GetPortSignals()[0]->GetSendingPort("O0")->GetParentComponent();
        }

        if(op->IsAttribution())
        {
            if(secondDependency->IsIoComponent())
            {
                context->AddStartValue(firstDependency, Component::GetIntegerValue(((IOComponent*) secondDependency)->GetValue()));
            }
            else
            {
                context->AddStartValue(firstDependency, Component::GetIntegerValue(parentSecondDependency->GetValue()));
            }
        }
        else if(op->IsBoolean())
        {
            context->SetLoopCondition(op);
            cout << "Set loop condition: '" << op->GetName() << "'" << endl;
        }
        else if(op->IsCompound())
        {
            if(secondDependency->IsIoComponent())
            {
                context->AddIncrementRate(firstDependency, Component::GetIntegerValue(((IOComponent*) secondDependency)->GetValue()));
            }
            else
            {
                context->AddIncrementRate(firstDependency, Component::GetIntegerValue(parentSecondDependency->GetValue()));
            }

            context->SetCycleClockCount(context->GetLoopClockCount());
            cout << "Set context " << context->GetName() << " clock cycles to " << context->GetCycleClockCount() << endl;
        }
    }

    return CXChildVisit_Recurse;
}

CXChildVisitResult NodeVisitor::VisitIntLiteral(NodeInfo* info)
{
    CXChildVisitResult result;

    string newInfoName = NodeInfo::GetTokenFromCursor(info->GetCursor(), 0);

    if(info->GetParentCursorKind() == CXCursor_VarDecl && NodeInfo::IsArray(info->GetParentCursorType()))
    {
        NodeInfo::PrintCursorInfo("Visiting int literal from array => ", info);
        Cmp_block_ram* ram = (Cmp_block_ram*) Visitor->GetDesign()->GetComponent("\\" + info->GetParentName() + "\\").IdentifiedComponent;
        ram->SetMaxSize(stoi(newInfoName));
        cout << info->GetParentName() << " max size = " << ram->GetMaxSize() << endl;
        result = CXChildVisit_Continue;
    }
    else if(info->GetParentCursorKind() == CXCursor_VarDecl && info->GetParentCursorTypeKind() == CXType_Int)
    {//TODO: Ajustar para trazer sa constantes das variables.
        NodeInfo::PrintCursorInfo("Visiting int literal from int => ", info);

        Component* parentComponent = Visitor->GetDesign()->GetComponent("\\" + info->GetParentName() + "\\").IdentifiedComponent;
        cout << "Component: " << parentComponent->GetInstanceName() + " (" + parentComponent->GetComponentName() + ")" << endl;

        if(parentComponent->IsIoComponent())
        {
            ((IOComponent*) parentComponent)->SetValue(Component::GetBinaryValue(stoll(newInfoName,0,10),parentComponent->GetDataWidth()));

            cout << "Added Cmp_counter: " << parentComponent->GetInstanceName() << " with value '" << ((IOComponent*) parentComponent)->GetValue() << "'" << endl;
        }

        string componentName = "const_" + newInfoName;//newInfoName + "_" + info->GetParentName();
        string componentPort;

        if(parentComponent->GetComponentName().compare("reg_op") == 0)
        {
            componentPort = "I0";
        }
        else if(parentComponent->GetComponentName().compare("counter") == 0)
        {
            componentPort = "input";
        }

        Cmp_const_op* constOp;

        if(Visitor->GetDesign()->HasComponent("\\" + componentName + "\\") && Visitor->GetDesign()->GetComponent("\\" + componentName + "\\").IdentifiedComponent->GetComponentName().compare("const_op") == 0)
        {
            constOp = (Cmp_const_op*) Visitor->GetDesign()->GetComponent("\\" + componentName + "\\").IdentifiedComponent;

            cout << "Connected '" << constOp->GetInstanceName() << "' " << " with value '" << constOp->GetValue() << " to '"
                << parentComponent->GetInstanceName() << "' (" << parentComponent->GetComponentName() << ") port '" << parentComponent->GetPort("I0")->GetName() << "'" << endl;
        }
        else
        {
            Signal* sig = new Signal("\\" + componentName + "\\", "std_logic_vector", Visitor->GetDesign()->GetWidth());
            constOp = new Cmp_const_op(Visitor->GetDesign()->GetWidth());
            constOp->SetRelatedSignal(sig);
            constOp->SetInstanceName("\\" + componentName + "\\");
            constOp->SetComponentName("const_op");
            constOp->SetValue(Component::GetBinaryValue(stoll(newInfoName,0,10),parentComponent->GetDataWidth()));

            Visitor->GetDesign()->AddDeclaredName(constOp->GetComponentName());
            Visitor->GetDesign()->AddComponent(constOp, info->GetCursor());
            Visitor->GetDesign()->AddToNumberOfComponents(constOp->GetComponentName());

            cout << "Added Cmp_const_op: " << constOp->GetInstanceName() << " with value '" << constOp->GetValue() <<
            "' and connected to '" << parentComponent->GetInstanceName() << "' (" << parentComponent->GetComponentName() << ") port '" << parentComponent->GetPort("I0")->GetName() << "'" << endl;
        }



        DependencyLevel* maxDep = parentComponent->GetMaxDependencyLevel(DependencyLevel::TYPE_WRITE);

        if(maxDep->GetLevel() == 0)
        {
            Visitor->GetDesign()->ConnectPortSignal(parentComponent, parentComponent->GetPort(componentPort), constOp->GetRelatedSignal(), false);
        }
        else
        {
            Visitor->GetDesign()->ConnectDelayedPortSignal(parentComponent, parentComponent->GetPort(componentPort), constOp->GetRelatedSignal(), false, maxDep->GetLevel() + parentComponent->GetContext()->GetStartClock());
        }

        result = CXChildVisit_Continue;
    }
    else
    {
        result = CXChildVisit_Recurse;
    }
    return result;
}

CXChildVisitResult NodeVisitor::VisitFor(NodeInfo* info)
{
    NodeInfo::PrintCursorInfo("Visiting 'for' => ", info);

    Visitor->GetDesign()->IncrementNumberOfFors();

    return CXChildVisit_Recurse;
}

CXChildVisitResult NodeVisitor::VisitIf(NodeInfo* info)
{
    NodeInfo::PrintCursorInfo("Visiting 'if' => ", info);

    return CXChildVisit_Recurse;
}

CXChildVisitResult NodeVisitor::VisitReturn(NodeInfo* info)
{
    NodeInfo::PrintCursorInfo("Visiting return => ", info);

    return CXChildVisit_Recurse;
}

CXChildVisitResult NodeVisitor::VisitCompoundStatement(NodeInfo* info)
{
    NodeInfo::PrintCursorInfo("Visiting compound statement => ", info);
    Context::GetLastContext(Visitor->GetContexts())->SetCompoundStatement(true);
    return CXChildVisit_Recurse;
}

CXChildVisitResult NodeVisitor::VisitFunction(NodeInfo* info)
{
    NodeInfo::PrintCursorInfo("Visiting function => ", info);

    if(info->GetParentCursorKind() == CXCursor_TranslationUnit && Visitor->GetDesign()->GetNumberOfFunctions() == 0)//If is function declaring and the first one declared.
    {
        Context* context = Context::GetContext(Visitor->GetContexts(), info->GetCursor());
        if(context->GetName().compare("main") == 0)
        {
            context->SetCycleClockCount(0);
            cout << "Set 0 clock cycles to main." << endl;
        }

        for(Variable* v : (*Visitor->GetVariables()))
        {
            if(v->GetRelatedComponent() != NULL && v->GetRelatedComponent()->GetComponentName().compare("const_op") == 0)
            {
                Visitor->GetDesign()->AddDeclaredName(v->GetRelatedComponent()->GetComponentName());
                Visitor->GetDesign()->AddComponent(v->GetRelatedComponent(), info->GetParentCursor());
                Visitor->GetDesign()->AddToNumberOfComponents(v->GetRelatedComponent()->GetComponentName());
                cout << "Added component constant from #define: " << v->GetRelatedComponent()->GetInstanceName() << " (" << v->GetRelatedComponent()->GetComponentName() << ", value: " << ((IOComponent*) v->GetRelatedComponent())->GetValue() <<")" << endl;
            }
        }
    }

    Visitor->GetDesign()->IncrementNumberOfFunctions();

	return CXChildVisit_Recurse;
}

vector<Cmp_const_op*> NodeVisitor::GetConstants(NodeInfo* info)
{
    vector<Cmp_const_op*> ret;
    vector<CXToken> tokens = info->GetParentCursorTokens();

    for(int i = 0; i < tokens.size(); i++)
    {
        string tokenName = NodeInfo::GetTokenName(Visitor->GetTranslationUnit(), tokens[i]);

        if(tokenName.compare("#") == 0)
        {
            string defineName = NodeInfo::GetTokenName(Visitor->GetTranslationUnit(), tokens[i+1]);
            if(defineName.compare("define") == 0)
            {
                string componentName = NodeInfo::GetTokenName(Visitor->GetTranslationUnit(), tokens[i+2]);
                string componentValue = NodeInfo::GetTokenName(Visitor->GetTranslationUnit(), tokens[i+3]);

                Signal* relatedSignal = new Signal("\\" + componentName + "\\", "std_logic_vector", Visitor->GetDesign()->GetWidth());

                Cmp_const_op* constOp = new Cmp_const_op(Visitor->GetDesign()->GetWidth());//Relates register to a constant.
                constOp->SetRelatedSignal(relatedSignal);
                constOp->SetComponentName("const_op");
                constOp->SetInstanceName("\\" + componentName + "\\");
                constOp->SetValue(Component::GetBinaryValue(stoll(componentValue,0,10),Visitor->GetDesign()->GetWidth()));

                ret.push_back(constOp);
            }

            i+=3;
        }
    }

    return ret;
}

CXChildVisitResult NodeVisitor::VisitCompoundAssignOperator(NodeInfo* info)
{
    Operation* operation = Visitor->GetOperation(info->GetCursor());
    Component* assigned = (*operation->GetDependencies())[0]->first;

    cout << "Compound assign operation: '" << operation->GetName() << "' to assign at '" << assigned->GetInstanceName() << "'" << " (" << assigned->GetComponentName() << ")" << endl;

    Component* componentOperation;

    if(operation->GetName().compare(NodeInfo::Operation_CompoundSum2) == 0)
    {
        componentOperation = new Cmp_add_op_s(Visitor->GetDesign()->GetWidth());
    }
    else if(operation->GetName().compare(NodeInfo::Operation_CompoundSubtraction2) == 0)
    {
        componentOperation = new Cmp_sub_op_s(Visitor->GetDesign()->GetWidth());
    }
    else if(operation->GetName().compare(NodeInfo::Operation_CompoundMultiplication) == 0)
    {
        componentOperation = new Cmp_mult_op_s(Visitor->GetDesign()->GetWidth());
    }else if(operation->GetName().compare(NodeInfo::Operation_CompoundDivision) == 0)
    {
        componentOperation = new Cmp_div_op_s(Visitor->GetDesign()->GetWidth());
    }
    else
    {
        throw invalid_argument("Invalid compound assignment operation '" + operation->GetName() + "': " + to_string(MessageLibrary::ERROR_UNEXPECTED_OPERATION));
    }

    componentOperation->GetMap("w_in1")->SetValue(to_string(Visitor->GetDesign()->GetWidth()));
    componentOperation->GetMap("w_in2")->SetValue(to_string(Visitor->GetDesign()->GetWidth()));
    componentOperation->GetMap("w_out")->SetValue(to_string(Visitor->GetDesign()->GetWidth()));

    string assignedInPortName;
    string assignedOutPortName;

    if(assigned->GetComponentName().compare("counter") == 0)
    {
        assignedInPortName = "input";
        assignedOutPortName = "output";
    }
    else if(assigned->GetComponentName().compare("reg_op") == 0)
    {
        assignedInPortName = "I0";
        assignedOutPortName = "O0";
    }
    else if(assigned->GetComponentName().compare("const_op") == 0)
    {
        assignedInPortName = "";//const_op may never be assigned
        assignedOutPortName = "O0";
        throw invalid_argument("Const_Op may never be assigned of any value at compound assign operation.");
    }
    else if(assigned->GetComponentName().compare("block_ram") == 0)
    {
        assignedInPortName = "data_in";
        assignedOutPortName = "data_out";
    }
    else
    {
        throw invalid_argument("Unexpected component name '" + assigned->GetComponentName() + " at VisitFirstExpression from parent = 'CXCursor_BinaryOperator'.");
    }

    componentOperation->SetInstanceName("\\" + componentOperation->GetComponentName() + "_" + to_string(Visitor->GetDesign()->GetNumberOfOperations()) + "\\");
    Visitor->GetDesign()->IncrementNumberOfOperations();

    Visitor->GetDesign()->AddDeclaredName(componentOperation->GetInstanceName());
    Visitor->GetDesign()->AddComponent(componentOperation, info->GetParentCursor());
    Visitor->GetDesign()->AddToNumberOfComponents(componentOperation->GetInstanceName());

    componentOperation->ConnectDefaultSignals(Visitor->GetDesign());
    operation->SetRelatedComponent(componentOperation);

    //Connection from assigned to operation
    string sigName = "from_" + StringUtils::Replace(assigned->GetInstanceName(), "\\", "") + "_to_" + StringUtils::Replace(componentOperation->GetInstanceName(), "\\", "");
    Signal* assignedToOperation = new Signal("\\" + sigName + "\\", "std_logic_vector", Visitor->GetDesign()->GetWidth());
    Visitor->GetDesign()->AddNamedSignal(assignedToOperation, sigName);
    Visitor->GetDesign()->ConnectPorts(componentOperation, componentOperation->GetPort("I0"), assigned, assigned->GetPort(assignedOutPortName), assignedToOperation);

    NodeInfo::PrintCursorInfo("Added the component '" + componentOperation->GetInstanceName() + "' (" + componentOperation->GetComponentName() + ") at parent => ", info);

    cout << "Created signal '" << assignedToOperation->GetSignalName() << "' and connected from '"
        << componentOperation->GetInstanceName() << "' (" << componentOperation->GetComponentName() << ") port '" << componentOperation->GetPort("I0")->GetName() << "' to '"
        << assigned->GetInstanceName() << "' (" << assigned->GetComponentName() << ") port '" << assigned->GetPort(assignedOutPortName)->GetName() << "'." << endl;

    //Recursive connection from operation to assigned
    sigName = "recursive_from_" + StringUtils::Replace(componentOperation->GetInstanceName(), "\\", "") + "_to_" + StringUtils::Replace(assigned->GetInstanceName(), "\\", "");
    Signal* operationToAssigned = new Signal("\\" + sigName + "\\", "std_logic_vector", Visitor->GetDesign()->GetWidth());
    Visitor->GetDesign()->AddNamedSignal(operationToAssigned, sigName);
    Visitor->GetDesign()->ConnectPorts(assigned, assigned->GetPort(assignedInPortName), componentOperation, componentOperation->GetPort("O0"), operationToAssigned);

    cout << "Created signal '" << operationToAssigned->GetSignalName() << "' and recursively connected from '"
        << assigned->GetInstanceName() << "' (" << assigned->GetComponentName() << ") port '" << assigned->GetPort(assignedInPortName)->GetName() << "' to '"
        << componentOperation->GetInstanceName() << "' (" << componentOperation->GetComponentName() << ") port '" << componentOperation->GetPort("O0")->GetName() << "'." << endl;
}

CXChildVisitResult NodeVisitor::VisitArraySubscript(NodeInfo* info)
{
    NodeInfo::PrintCursorInfo("Visiting array subscript => ", info);
    return CXChildVisit_Recurse;
}

//Method used to visit the node of a CLang CXCursor, called at 'Synthesize' method.
CXChildVisitResult NodeVisitor::VisitNode(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    NodeInfo* info = new NodeInfo(cursor, parent, client_data);
    Visitor->AddInfoHistory(info);
    //NodeInfo::PrintHistory(info->GetCursorHistory());

    Context::CheckAddContext(info, Visitor->GetContexts());
	Context::CheckRemoveContext(info, Visitor->GetContexts());

    if(info->IsFromMainFile())
    {
        return CXChildVisit_Recurse;
    }

	return ChooseVisitor(info);
}

CXChildVisitResult NodeVisitor::FirstVisitation(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    CXChildVisitResult result;
    NodeInfo* info = new NodeInfo(cursor, parent, client_data);

    if(info->IsFromMainFile())
    {
        return CXChildVisit_Recurse;
    }
    //NodeInfo::PrintCursorInfo(info);
    string operation;
	Operation* op;
	Component* relatedComponent = NULL;

    Variable* foundVariable = Visitor->GetVariable(info->GetName());

    if(info->GetParentCursorKind() == CXCursor_TranslationUnit && Visitor->GetVariables()->size() == 0)//If is function declaring and the first one declared.
    {
        vector<Cmp_const_op*> consts = GetConstants(info);
        for(Cmp_const_op* constOp : consts)
        {
            constOp->SetUniversal(true);
            Visitor->AddVariable(StringUtils::Replace(constOp->GetInstanceName(),"\\", ""), constOp, info->GetParentCursor());
        }

        result = CXChildVisit_Recurse;
    }
    else
    {
        switch(info->GetCursorKind())
        {
            case CXCursor_VarDecl:
                if(NodeInfo::IsArray(info->GetCursorType()))
                {
                    Cmp_block_ram* ram = new Cmp_block_ram(Visitor->GetDesign()->GetWidth());
                    ram->SetInstanceName("\\" + info->GetName() + "\\");
                    ram->SetComponentName("block_ram");
                    ram->GetMap("data_width")->SetValue(to_string(Visitor->GetDesign()->GetWidth()));
                    ram->GetMap("address_width")->SetValue(to_string(Visitor->GetDesign()->GetWidth()));
                    Visitor->AddVariable(info->GetName(), ram, cursor);
                    ram->SetId(Visitor->GetVariables()->size());
                    result = CXChildVisit_Continue;
                }
                else if(info->GetCursorType().kind == CXType_Int)
                {
                    Cmp_reg_op* reg = new Cmp_reg_op(Visitor->GetDesign()->GetWidth());
                    reg->SetInstanceName("\\" + info->GetName() + "\\");
                    reg->SetComponentName("reg_op");
                    reg->GetMap("w_in")->SetValue(to_string(Visitor->GetDesign()->GetWidth()));
                    reg->GetMap("initial")->SetValue(to_string(0));
                    Visitor->AddVariable(info->GetName(), reg, cursor);
                    reg->SetId(Visitor->GetVariables()->size());
                    result = CXChildVisit_Continue;
                }
                else
                {
                    result = CXChildVisit_Recurse;
                }
                break;
            case CXCursor_IntegerLiteral:
                op = Visitor->GetOperation(info->GetParentCursor());

                if(op->GetName().compare("=") == 0)
                {
                    string value = NodeInfo::GetTokenFromCursor(info->GetCursor(), 0);
                    operation = "\\const_" + value + "\\";

                    relatedComponent = NodeVisitor::GetOperationDependency(operation, Visitor->GetOperations());

                    if(relatedComponent == NULL)
                    {
                        relatedComponent = new Cmp_const_op(operation, Visitor->GetDesign()->GetWidth());
                        relatedComponent->SetInstanceName(operation);
                        relatedComponent->SetComponentName("const_op");
                        Signal* sig = new Signal("\\" + operation + "\\", "std_logic_vector", Visitor->GetDesign()->GetWidth());
                        ((Cmp_const_op*) relatedComponent)->SetRelatedSignal(sig);
                        ((IOComponent*) relatedComponent)->SetValue(Component::GetBinaryValue(StringUtils::StringToInt(value), relatedComponent->GetDataWidth()));
                    }

                    Visitor->AddVariable(operation, relatedComponent, info->GetCursor());
                    op->AddDependency(relatedComponent, -1);
                    cout << "Added dependency '" << relatedComponent->GetInstanceName() << "' (id = " << relatedComponent->GetId() << ", " << relatedComponent->GetComponentName() << ") to operation '" << op->GetName() << "'" << endl;
                }
                break;
            case CXCursor_DeclRefExpr:
                //NodeInfo::PrintCursorInfo(info);
                if(info->GetParentCursorKind() == CXCursor_BinaryOperator)
                {
                    op = Visitor->GetOperation(info->GetParentCursor());

                    if(op->GetName().compare("=") == 0)
                    {
                        operation = NodeInfo::GetTokenFromCursor(info->GetCursor(), 0);
                        relatedComponent = Visitor->GetVariable(operation)->GetRelatedComponent();
                        op->AddDependency(relatedComponent, -1);
                        cout << "Added dependency '" << relatedComponent->GetInstanceName() << "' (id = " << relatedComponent->GetId() << ", " << relatedComponent->GetComponentName() << ") to operation '" << op->GetName() << "'" << endl;
                    }

                    if(relatedComponent != NULL && relatedComponent->IsBlockRam() && NodeInfo::GetTokenFromCursor(info->GetCursor(), 1).compare("[") == 0)
                    {
                        Component* subdependency = Visitor->GetVariable(NodeInfo::GetTokenFromCursor(info->GetCursor(), 2))->GetRelatedComponent();
                        int addr = Visitor->GetOperation(info->GetParentCursor())->GetDependencyAddress(relatedComponent->GetInstanceName(), -1);
                        op->AddDependency(subdependency, addr);
                        cout << "Added subdependency '" << subdependency->GetInstanceName() << "' of '" << addr << "' to operation '" << op->GetName() << "'" << endl;
                    }

                    result = CXChildVisit_Recurse;
                }
                else if(info->GetParentCursorKind() == CXCursor_CompoundAssignOperator)
                {
                    op = Visitor->GetOperation(info->GetParentCursor());

                    string comp = NodeInfo::GetTokenFromCursor(info->GetCursor(), 0);
                    Component* compoundComp = Visitor->GetVariable(comp)->GetRelatedComponent();

                    op->AddDependency(compoundComp, 0);

                    cout << "Added dependency '" << compoundComp->GetInstanceName() << "' (id = " << compoundComp->GetId() << ", " << compoundComp->GetComponentName() << ") to operation '" << op->GetName() << "'" << endl;

                    if(compoundComp->IsBlockRam() && NodeInfo::GetTokenFromCursor(info->GetCursor(), 1).compare("[") == 0)
                    {
                        Component* subdependency = Visitor->GetVariable(NodeInfo::GetTokenFromCursor(info->GetCursor(), 2))->GetRelatedComponent();
                        int addr = Visitor->GetOperation(info->GetParentCursor())->GetDependencyAddress(compoundComp->GetInstanceName(), -1);
                        op->AddDependency(subdependency, addr);
                        cout << "Added subdependency '" << subdependency->GetInstanceName() << "' of '" << addr << "' to operation '" << op->GetName() << "'" << endl;
                    }

                    result = CXChildVisit_Recurse;
                }
                else
                {
                    string operation = NodeInfo::GetTokenFromCursor(info->GetCursor(), -1);
                    if(operation.compare(";") == 0)
                    {
                        operation = NodeInfo::GetTokenFromCursor(info->GetCursor(), 0);
                    }

                    if(Operation::IsCompoundOperation(operation) && info->GetParentCursorKind() == CXCursor_UnaryOperator)
                    {
                        Component* currentComponent = foundVariable->GetRelatedComponent();

                        if(currentComponent->GetComponentName().compare("reg_op") == 0)
                        {
                            Cmp_counter* counter = new Cmp_counter(Visitor->GetDesign()->GetWidth());
                            counter->SetInstanceName(currentComponent->GetInstanceName());
                            counter->SetComponentName("counter");
                            counter->GetMap("bits")->SetValue(to_string(Visitor->GetDesign()->GetWidth()));
                            counter->GetMap("steps")->SetValue(to_string(Visitor->GetDesign()->GetWidth()));
                            counter->SetId(currentComponent->GetId());
                            foundVariable->SetRelatedComponent(counter);
                            Operation::ChangeDependency(Visitor->GetOperations(), counter->GetInstanceName(), counter);

                            op = new Operation(operation, NULL, info->GetParentCursor());
                            op->AddDependency(counter, -1);
                            Visitor->AddOperation(op);
                        }

                        result = CXChildVisit_Continue;
                    }
                    else if(foundVariable != NULL)
                    {
                        foundVariable->GetRelatedComponent()->GetMap("w_in")->SetValue(to_string(Visitor->GetDesign()->GetWidth()));
                        foundVariable->GetRelatedComponent()->GetMap("initial")->SetValue(to_string(0));
                        result = CXChildVisit_Recurse;
                    }
                    else
                    {
                        result = CXChildVisit_Recurse;
                    }
                }
                break;
            case CXCursor_FirstExpr:
                //NodeInfo::PrintCursorInfo(info);
                if(info->GetParentCursorKind() == CXCursor_BinaryOperator)
                {
                    operation = NodeInfo::GetTokenFromCursor(info->GetCursor(), -1);
                    op = Visitor->GetOperation(info->GetParentCursor());
                    Component* dependency = NULL;

                    if(op == NULL)
                    {
                        Component* comp = NodeInfo::GetComponent(operation, Visitor->GetDesign()->GetWidth());

                        if(comp != NULL)
                        {
                            comp->SetInstanceName("\\" + comp->GetComponentName() + "_" + to_string(Visitor->GetOperations()->size()) + "\\");
                        }

                        op = new Operation(operation, comp, info->GetParentCursor());
                        Visitor->GetOperations()->push_back(op);
                        cout << "Added operation '" << op->GetName() << "' - " << (op->GetRelatedComponent() == NULL ? "NULL" : op->GetRelatedComponent()->GetComponentName()) << endl;
                    }

                    dependency = Visitor->GetVariable(NodeInfo::GetTokenFromCursor(info->GetCursor(), 0))->GetRelatedComponent();

                    op->AddDependency(dependency, -1);
                    cout << "Added dependency '" << dependency->GetInstanceName() << "' (id = " << dependency->GetId() << ", " << dependency->GetComponentName()
                        << ") to operation '" << op->GetName() << "' (" << (op->GetRelatedComponent() == NULL ? "NULL" : op->GetRelatedComponent()->GetComponentName()) << ")" << endl;

                    for(CXToken t : info->GetParentCursorTokens())
                    {
                        string s = NodeInfo::GetTokenName(Visitor->GetTranslationUnit(), t);

                        if(s.compare(op->GetName()) != 0 && s.compare(StringUtils::Replace(dependency->GetInstanceName(),"\\","")) != 0 && s.compare(";") != 0)
                        {
                            Variable* nextDependencyVar = Visitor->GetVariable(s);

                            if(nextDependencyVar != NULL && nextDependencyVar->GetRelatedComponent()->GetComponentName().compare("const_op") == 0)
                            {
                                Component* nextDependency = nextDependencyVar->GetRelatedComponent();
                                op->AddDependency(nextDependency, -1);
                                cout << "Added next dependency '" << nextDependency->GetInstanceName() << "' (id = " << nextDependency->GetId() << ", " << nextDependency->GetComponentName()
                                    << ") to operation '" << op->GetName() << "' (" << op->GetRelatedComponent()->GetInstanceName() << ")" << endl;

                            }
                        }
                    }

                    if(dependency->IsBlockRam() && NodeInfo::GetTokenFromCursor(info->GetCursor(), 1).compare("[") == 0)
                    {
                        Component* subdependency = Visitor->GetVariable(NodeInfo::GetTokenFromCursor(info->GetCursor(), 2))->GetRelatedComponent();
                        int addr = Visitor->GetOperation(info->GetParentCursor())->GetDependencyAddress(dependency->GetInstanceName(), -1);
                        op->AddDependency(subdependency, addr);
                        cout << "Added subdependency '" << subdependency->GetInstanceName() << "' of '" << addr << "' to operation '" << op->GetName() << "'" << endl;
                    }
                }
                result = CXChildVisit_Recurse;
                break;
            case CXCursor_BinaryOperator:
                //NodeInfo::PrintCursorInfo(info);
                operation = NodeInfo::GetOperation(info->GetCursor());
                op = new Operation(operation, NodeInfo::GetComponent(operation, Visitor->GetDesign()->GetWidth()), info->GetCursor());
                if(op->GetRelatedComponent() != NULL)
                {
                    op->GetRelatedComponent()->SetInstanceName("\\" + op->GetRelatedComponent()->GetComponentName() + "_" + to_string(Visitor->GetOperations()->size()) + "\\");
                }
                Visitor->AddOperation(op);
                cout << "Added operation '" << op->GetName() << "' (" << (op->GetRelatedComponent() == NULL ? "NULL" : op->GetRelatedComponent()->GetComponentName()) << ")" << endl;

                if(info->GetParentCursorKind() == CXCursor_BinaryOperator || info->GetParentCursorKind() == CXCursor_CompoundAssignOperator)
                {
                    Operation* parentBinOp = Visitor->GetOperation(info->GetParentCursor());

                    if(parentBinOp == NULL)
                    {
                        //yet not happended, so not implemented
                    }

                    parentBinOp->AddDependency(op->GetRelatedComponent(), -1);
                    cout << "Added dependency '" << op->GetName() << "' - " << op->GetRelatedComponent()->GetInstanceName() << " at operation '" << parentBinOp->GetName() << "'" << endl;
                }
                result = CXChildVisit_Recurse;
                break;
            case CXCursor_CompoundAssignOperator:
                operation = NodeInfo::GetOperation(info->GetCursor());
                relatedComponent = NodeInfo::GetComponent(operation, Visitor->GetDesign()->GetWidth());
                relatedComponent->SetInstanceName("\\" + relatedComponent->GetComponentName() + "_" + to_string(Visitor->GetOperations()->size()) + "\\");
                op = new Operation(operation, relatedComponent, info->GetCursor());
                Visitor->AddOperation(op);
                cout << "Added operation '" << op->GetName() << "' (" << op->GetRelatedComponent()->GetComponentName() << ")" << endl;

                result = CXChildVisit_Recurse;
                break;
            default:
                //NodeInfo::PrintCursorInfo(info);
                result = CXChildVisit_Recurse;
                break;
        }
    }

	return result;
}

CXChildVisitResult NodeVisitor::DependencyVisitation(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    CXChildVisitResult result;
    NodeInfo* info = new NodeInfo(cursor, parent, client_data);
    Visitor->AddInfoHistory(info);

    if(info->IsFromMainFile())
    {
        return CXChildVisit_Recurse;
    }
    //NodeInfo::PrintCursorInfo(info);
    Variable* cursorRelated = Visitor->GetVariable(info->GetName());
    Component* componentRelated = NULL;
    Variable* parentRelated = Visitor->GetVariable(info->GetParentCursor());
    Component* parentComponentRelated = NULL;
    NodeInfo* grandParent = info->GetCursorHistory(NodeInfo::History_2_Levels_Above);
    DependencyLevel* newDependencyLevel = NULL;

    Context::CheckAddContext(info, Visitor->GetContexts());
    Context::CheckRemoveContext(info, Visitor->GetContexts());

    if(Visitor->GetVariable(info->GetCursor()) != NULL)//Only for declarations
    {
        //NodeInfo::PrintCursorInfo("CursorRelated not NULL: '" + cursorRelated->GetRelatedComponent()->GetInstanceName() + "' (" + cursorRelated->GetRelatedComponent()->GetComponentName() + "): ", info);

        newDependencyLevel = Visitor->GetDesign()->GetAndCreateDependencyLevel(0, DependencyLevel::TYPE_WRITE);

        newDependencyLevel->AddDependency(cursorRelated->GetRelatedComponent());

        cout << "Added " << cursorRelated->GetRelatedComponent()->GetInstanceName()
            << " (" + cursorRelated->GetRelatedComponent()->GetComponentName() << ", id = " << cursorRelated->GetRelatedComponent()->GetId()
            << ") to dependency level " << newDependencyLevel->GetLevel() << " of type " << newDependencyLevel->GetType() << endl;
    }
    else if(info->GetCursorKind() == CXCursor_DeclRefExpr && info->GetParentCursorKind() ==  CXCursor_FirstExpr && grandParent->GetCursorKind() == CXCursor_VarDecl)//int i = n (sample)
    {
        //NodeInfo::PrintCursorInfo("GrandParent: ", grandParent);
        Variable* grandParentCursorRelated = Visitor->GetVariable(grandParent->GetName());
        componentRelated = cursorRelated->GetRelatedComponent();

        if(grandParentCursorRelated != NULL)
        {
            Component* grandParentComponent = grandParentCursorRelated->GetRelatedComponent();
            newDependencyLevel = Visitor->GetDesign()->GetAndCreateDependencyLevel(componentRelated->GetMaxDependencyLevel(DependencyLevel::TYPE_WRITE)->GetLevel() + 1, DependencyLevel::TYPE_WRITE);

            if(grandParentComponent->GetMaxDependencyLevel(DependencyLevel::TYPE_WRITE)->GetLevel() <= componentRelated->GetMaxDependencyLevel(DependencyLevel::TYPE_WRITE)->GetLevel())
            {
                DependencyLevel::MoveDependency(grandParentComponent->GetMaxDependencyLevel(DependencyLevel::TYPE_WRITE), newDependencyLevel, grandParentComponent->GetInstanceName());
            }
        }
    }
    else if(info->GetCursorKind() == CXCursor_DeclRefExpr && info->GetParentCursorKind() == CXCursor_VarDecl)
    {
        //NodeInfo::PrintCursorInfo(info);
        parentComponentRelated = parentRelated->GetRelatedComponent();

        if(parentRelated != NULL && parentComponentRelated->IsBlockRam())
        {
            Component* ramSize = Visitor->GetVariable(NodeInfo::GetTokenFromCursor(info->GetCursor(), 0))->GetRelatedComponent();
            if(ramSize != NULL)
            {
                newDependencyLevel = Visitor->GetDesign()->GetAndCreateDependencyLevel(ramSize->GetMaxDependencyLevel(DependencyLevel::TYPE_WRITE)->GetLevel() + 1, DependencyLevel::TYPE_WRITE);

                if(parentComponentRelated->GetMaxDependencyLevel(DependencyLevel::TYPE_WRITE)->GetLevel() <= ramSize->GetMaxDependencyLevel(DependencyLevel::TYPE_WRITE)->GetLevel())
                {
                    DependencyLevel::MoveDependency(parentComponentRelated->GetMaxDependencyLevel(DependencyLevel::TYPE_WRITE), newDependencyLevel, parentComponentRelated->GetInstanceName());
                }
            }
        }
    }
    else if(info->GetCursorKind() == CXCursor_BinaryOperator || info->GetCursorKind() == CXCursor_UnaryOperator)
    {
        //NodeInfo::PrintCursorInfo(info);
        DependencyLevel* maxLvl = NULL;
        Operation* operation = Visitor->GetOperation(info->GetCursor());

        for(pair<Component*,int>* p : (*operation->GetDependencies()))
        {
            DependencyLevel* maxPLvl = p->first->GetMaxDependencyLevel();

            if(maxPLvl != NULL && (maxLvl == NULL || (maxLvl != NULL && maxPLvl->GetLevel() > maxLvl->GetLevel())))
            {
                maxLvl = p->first->GetMaxDependencyLevel(DependencyLevel::TYPE_WRITE);
            }
        }

        DependencyLevel* nextLevel = NULL;

        if(info->GetParentCursorKind() == CXCursor_BinaryOperator || info->GetParentCursorKind() == CXCursor_CompoundAssignOperator)
        {
            Operation* parentOperation = Visitor->GetOperation(info->GetParentCursor());

            DependencyLevel* parentLevel;

            if(parentOperation->GetRelatedComponent() != NULL)
            {
                parentLevel = parentOperation->GetRelatedComponent()->GetMaxDependencyLevel();
            }
            else
            {
                parentLevel = parentOperation->GetDependencyLevel();
            }

            if(maxLvl == NULL && parentLevel == NULL)
            {
                nextLevel = Visitor->GetDesign()->GetAndCreateDependencyLevel(0, DependencyLevel::TYPE_READ);
            }
            else if(maxLvl == NULL)
            {
                nextLevel = Visitor->GetDesign()->GetAndCreateDependencyLevel(parentLevel->GetLevel(), DependencyLevel::TYPE_READ);
            }
            else
            {
                nextLevel = Visitor->GetDesign()->GetAndCreateDependencyLevel((maxLvl->GetLevel() + 1 > parentLevel->GetLevel() ? maxLvl->GetLevel() + 1 : parentLevel->GetLevel()), DependencyLevel::TYPE_READ);
            }

            if(parentLevel->GetLevel() <= nextLevel->GetLevel())
            {
                DependencyLevel* oldParentLevel = parentLevel;
                parentLevel = Visitor->GetDesign()->GetAndCreateDependencyLevel(nextLevel->GetLevel() + 1, DependencyLevel::TYPE_READ);

                Operation* parentOperation = Visitor->GetOperation(info->GetParentCursor());

                if(parentOperation->GetRelatedComponent() != NULL && oldParentLevel->GetDependency(parentOperation->GetRelatedComponent()->GetInstanceName()) != NULL)
                {//cout << "3: " << parentOperation->GetRelatedComponent()->GetInstanceName() << endl;
                    //parentOperation->GetRelatedComponent()->AddDependencyLevel(nextLevel);
                    DependencyLevel::MoveDependency(oldParentLevel, parentLevel, parentOperation->GetRelatedComponent()->GetInstanceName());
                }
                else if(parentOperation->GetRelatedComponent() != NULL && parentLevel->GetType() == DependencyLevel::TYPE_WRITE)
                {
                    parentLevel->AddDependency(parentOperation->GetRelatedComponent());
                    parentOperation->GetRelatedComponent()->AddDependencyLevel(parentLevel);
                    cout << "Added " << parentOperation->GetRelatedComponent()->GetInstanceName() << " to level " << parentLevel->GetLevel() << endl;
                }

                parentOperation->SetDependencyLevel(parentLevel);
            }
        }
        else
        {
            if(maxLvl == NULL)
            {
                nextLevel = Visitor->GetDesign()->GetAndCreateDependencyLevel(0, DependencyLevel::TYPE_READ);
            }
            else
            {
                nextLevel = Visitor->GetDesign()->GetAndCreateDependencyLevel(maxLvl->GetLevel() + 1, DependencyLevel::TYPE_READ);
            }
        }

        if(operation->GetRelatedComponent() != NULL)
        {
            operation->GetRelatedComponent()->AddDependencyLevel(nextLevel);
            operation->SetDependencyLevel(nextLevel);
        }
        else
        {
            nextLevel = nextLevel == NULL ? Visitor->GetDesign()->GetAndCreateDependencyLevel(0, DependencyLevel::TYPE_READ) : nextLevel;
            operation->SetDependencyLevel(nextLevel);
        }

        cout << "Added " << operation->GetName() << " to dependency level " << operation->GetDependencyLevel()->GetLevel() << " of type " << operation->GetDependencyLevel()->GetType() << endl;
    }
    else if(info->GetCursorKind() == CXCursor_CompoundAssignOperator)
    {
        //NodeInfo::PrintCursorInfo(info);
        DependencyLevel* maxLvl = NULL;
        Operation* operation = Visitor->GetOperation(info->GetCursor());

        for(pair<Component*,int>* p : (*operation->GetDependencies()))
        {
            DependencyLevel* maxPLvl = p->first->GetMaxDependencyLevel();

            if(maxPLvl != NULL && (maxLvl == NULL || (maxLvl != NULL && maxPLvl->GetLevel() > maxLvl->GetLevel())))
            {
                maxLvl = p->first->GetMaxDependencyLevel(DependencyLevel::TYPE_WRITE);
            }
        }

        DependencyLevel* nextLevel = NULL;

        if(maxLvl == NULL)
        {
            nextLevel = Visitor->GetDesign()->GetAndCreateDependencyLevel(0, DependencyLevel::TYPE_READ);
        }
        else
        {
            nextLevel = Visitor->GetDesign()->GetAndCreateDependencyLevel(maxLvl->GetLevel() + 1, DependencyLevel::TYPE_READ);
        }

        if(operation->GetRelatedComponent() != NULL)
        {
            operation->GetRelatedComponent()->AddDependencyLevel(nextLevel);
            operation->SetDependencyLevel(nextLevel);
        }
        else
        {
            operation->SetDependencyLevel(nextLevel == NULL ? Visitor->GetDesign()->GetAndCreateDependencyLevel(0, DependencyLevel::TYPE_READ) : nextLevel);
        }

        cout << "Added " << operation->GetName() << " to dependency level " << nextLevel->GetLevel() << " of type " << nextLevel->GetType() << endl;
    }
    else if(info->GetCursorKind() == CXCursor_FirstExpr &&
        (
            info->GetParentCursorKind() == CXCursor_BinaryOperator ||
            info->GetCursorHistory(NodeInfo::History_2_Levels_Above)->GetCursorKind() == CXCursor_BinaryOperator ||
            info->GetCursorHistory(NodeInfo::History_3_Levels_Above)->GetCursorKind() == CXCursor_BinaryOperator
        )
    )
    {
        if(info->GetParentCursorKind() == CXCursor_ArraySubscriptExpr)
        {
            //NodeInfo::PrintCursorInfo("FirstExpr/BinaryOperator/ArraySubscript: ", info);
        }
        else
        {
            //NodeInfo::PrintCursorInfo("FirstExpr/BinaryOperator: ", info);
        }

    }
    else if(info->GetCursorKind() == CXCursor_DeclRefExpr && info->GetParentCursorKind() == CXCursor_CompoundAssignOperator)
    {
        //NodeInfo::PrintCursorInfo("DeclRef/CompoundAssign: ", info);
    }
    else
    {
        //NodeInfo::PrintCursorInfo(info);
    }

    result = CXChildVisit_Recurse;

    return result;
}

CXTranslationUnit NodeVisitor::Initialize(char* inputPath)
{
    //http://clang.llvm.org/doxygen/group__CINDEX.html#ga51eb9b38c18743bf2d824c6230e61f93
    //Provides a shared context for the translation units.
    CXIndex index = clang_createIndex(0, 0);

    if(index == 0)
    {
        cerr << "Synthesis error \"ERROR_CREATING_CX_INDEX\" (" << MessageLibrary::ERROR_CREATING_CX_INDEX << ")." << endl;
        throw invalid_argument(to_string(MessageLibrary::ERROR_CREATING_CX_INDEX));
    }

    //http://clang.llvm.org/doxygen/group__CINDEX__TRANSLATION__UNIT.html#ga2baf83f8c3299788234c8bce55e4472e
    //Parse the given source file and the translation unit corresponding to that file.
    CXTranslationUnit translationUnit = clang_parseTranslationUnit(index, inputPath, NULL, 0, 0, 0, CXTranslationUnit_None);
    clang_disposeIndex(index);

    if(translationUnit == 0)
    {
        cerr << "Synthesis error \"ERROR_CREATING_CX_TRANSLATION_UNIT\" (" << MessageLibrary::ERROR_CREATING_CX_TRANSLATION_UNIT << ")." << endl;
        throw invalid_argument(to_string(MessageLibrary::ERROR_CREATING_CX_TRANSLATION_UNIT));
    }
    else
    {
        return translationUnit;
    }
}

//Method used to visit the node of a CLang CXCursor, called at 'Synthesize' method.
Design* NodeVisitor::VisitAll(char* inputPath, int width)
{
    free(Visitor);
    Visitor = new NodeVisitor();

    //Design return object
    Visitor->SetDesign(new Design());
    Visitor->GetDesign()->SetWidth(width);

    CXTranslationUnit translationUnit = Visitor->Initialize(inputPath);
    CXCursor rootCursor = clang_getTranslationUnitCursor(translationUnit);
    Visitor->SetTranslationUnit(translationUnit);

    unsigned int clangResultFirstVisitation = clang_visitChildren(rootCursor, FirstVisitation, 0);

    if(clangResultFirstVisitation == 0)
    {
        clang_visitChildren(rootCursor, DependencyVisitation, 0);

        cout << endl << "*****DEPENDENCY CHECK *****" << endl;
        for(Operation* o : (*Visitor->GetOperations()))
        {
            Component* relComp = o->GetRelatedComponent();
            cout << "Operation " << o->GetName() << " (" << (relComp != NULL ? relComp->GetInstanceName() : "NULL") << ", Level: " + to_string(o->GetDependencyLevel()->GetLevel()) << "): ";

            for(pair<Component*,int>* dep : (*o->GetDependencies()))
            {
                cout << dep->first->GetInstanceName() << " ";
            }
            cout << endl;
        }

        cout << endl;

        for(DependencyLevel* lvl : (*Visitor->GetDesign()->GetDependencies()))
        {
            cout << "DepLevel " << lvl->GetLevel() << ", " << lvl->GetType() << ": ";

            for(Component* comp : lvl->GetDependencies())
            {
                cout << comp->GetInstanceName() << " ";
            }
            cout << endl;
        }

        cout << "*****END OF DEPENDENCY CHECK *****" << endl << endl;

        Visitor->ClearInfoHistory();
        Visitor->ClearContexts();

        unsigned int clangResultFinal = clang_visitChildren(rootCursor, VisitNode, 0);//Suspended while testing dependencies.

        if(clangResultFinal == 0)
        {
            return Visitor->GetDesign();
        }
        else
        {
            cerr << "Synthesis error \"ERROR_VISITING_NODES\" (" << MessageLibrary::ERROR_VISITING_NODES << ", CLang internal: " << clangResultFinal << ")." << endl;
            throw invalid_argument(to_string(MessageLibrary::ERROR_VISITING_NODES));
        }
    }
    else
    {
        cerr << "Synthesis error \"ERROR_VISITING_NODES\" (" << MessageLibrary::ERROR_VISITING_NODES << ", CLang internal: " << clangResultFirstVisitation << ")." << endl;
        throw invalid_argument(to_string(MessageLibrary::ERROR_VISITING_NODES));
    }
}

void NodeVisitor::ClearInfoHistory()
{
    GetInfoHistory()->clear();
}

void NodeVisitor::ClearContexts()
{
    GetContexts()->clear();
}

void NodeVisitor::AddInfoHistory(NodeInfo* info)
{
    info->SetClientData(info->GetNextClientData());
    NodeInfo::Push(Visitor->GetInfoHistory(), info);
    info->SetCursorHistory(NodeInfo::GetPreviousHistory(Visitor->GetInfoHistory(), info->GetParentCursor()));
}

void NodeVisitor::AddOperation(Operation* op)
{
    if(op != NULL && Visitor->GetOperation(op->GetRelatedCursor()) == NULL)
    {
        Visitor->GetOperations()->push_back(op);
    }
    else
    {
        throw invalid_argument("Repeated or null operations are not allowed at visitor.");
    }
}

Operation* NodeVisitor::GetOperation(string comp)
{
    for(Operation* op : (*Visitor->GetOperations()))
    {
        if(op->GetRelatedComponent()->GetInstanceName().compare(comp) == 0)
        {
            return op;
        }
    }
    return NULL;
}

Operation* NodeVisitor::GetOperation(CXCursor cursor)
{
    for(Operation* op : (*Visitor->GetOperations()))
    {
        if(clang_hashCursor(op->GetRelatedCursor()) == clang_hashCursor(cursor))
        {
            return op;
        }
    }
    return NULL;
}

vector<Operation*>* NodeVisitor::GetOperations()
{
    return Operations;
}

Component* NodeVisitor::GetOperationDependency(string name, vector<Operation*>* operations)
{
    for(Operation* op : (*Visitor->GetOperations()))
    {
        for(pair<Component*,int>* dependency : (*op->GetDependencies()))
        {
            Component* dep = dependency->first;
            if(dep->GetInstanceName().compare(name) == 0)
            {
                return dep;
            }
        }
    }
    return NULL;
}

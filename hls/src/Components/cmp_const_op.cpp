#include<iostream>

#include<Components/cmp_const_op.hpp>

//protected Long value = 0;

Cmp_const_op::Cmp_const_op(string value, int width) : IOComponent() {
	SetComponentName("Cmp_const_op");
	SetDataWidth(width);
    CreatePorts();
    CreateGenericMaps();
    SetType("in");
	SetValue(value);
	SetCycleClockCount(0);
	//Port* p = new Port(GetComponentName(), width, "out", "std_logic_vector");
	//SetPort(p);
}

Cmp_const_op::Cmp_const_op(int width) : IOComponent() {
	SetComponentName("Cmp_const_op");
	SetDataWidth(width);
    CreatePorts();
    CreateGenericMaps();
    SetType("out");
    //Port* p = new Port(GetComponentName(), width, "out", "std_logic_vector");
	//SetPort(p);
}

void Cmp_const_op::CreatePorts() {
    Port* p = new Port("O0", GetDataWidth(), "out", "std_logic_vector");
    AddPort(p);
    SetPort(p);
    PortStrings["O0"] = 0;
}

// Instantiates all maps of the component
void Cmp_const_op::CreateGenericMaps() {
    AddGenericMap(new GenericMap("w_out", "integer", to_string(GetDataWidth())));
    MapStrings["w_out"] = 0;
}

string Cmp_const_op::GetVHDLDeclaration(string nameExtension)
{
    string decl = "";

    return decl;
}

void Cmp_const_op::SetRelatedSignal(Signal* relatedSignal)
{
    RelatedSignal = relatedSignal;
}

Signal* Cmp_const_op::GetRelatedSignal()
{
    return RelatedSignal;
}

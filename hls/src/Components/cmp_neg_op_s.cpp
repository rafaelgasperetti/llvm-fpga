#include<string>
#include<Core/Component.hpp>
#include<Components/cmp_neg_op_s.hpp>

Cmp_neg_op_s::Cmp_neg_op_s(int dataWidth) : Component() {
    SetComponentName("neg_op_s");
    SetDataWidth(dataWidth);
    CreatePorts();
    CreateGenericMaps();
    SetCycleClockCount(0);
}

// Returns the vhdl decl of the component
string Cmp_neg_op_s::GetVHDLDeclaration(string nameExtension){
    string decl = "";

    decl += "component " + GetComponentName() + "\n";
    decl += "generic (\n";
    decl += "	w_in	: integer := 16;\n";
    decl += "	w_out	: integer := 16\n";
    decl += ");\n";
    decl += "port (\n";
    decl += "	I0	: in	std_logic_vector(w_in-1 downto 0);\n";
    decl += "	O0	: out	std_logic_vector(w_out-1 downto 0)\n";
    decl += ");\n";
    decl += "end component;\n";
    decl += "\n";

    SetDeclared(true);

    return decl;
}

// Instantiates all ports of the component
void Cmp_neg_op_s::CreatePorts() {
    AddPort(new Port("I0", GetDataWidth(), "in", "std_logic_vector"));
    PortStrings["I0"] = 0;
    AddPort(new Port("O0", GetDataWidth(), "out", "std_logic_vector"));
    PortStrings["O0"] = 1;
}

// Instantiates all maps of the component
void Cmp_neg_op_s::CreateGenericMaps() {
    AddGenericMap(new GenericMap("w_in", "integer", to_string(GetDataWidth())));
    MapStrings["w_in"] = 0;
    AddGenericMap(new GenericMap("w_out", "integer", to_string(GetDataWidth())));
    MapStrings["w_out"] = 1;
}

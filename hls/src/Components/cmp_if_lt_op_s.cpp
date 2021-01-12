#include<string>
#include<Core/Component.hpp>
#include<Components/cmp_if_lt_op_s.hpp>

Cmp_if_lt_op_s::Cmp_if_lt_op_s(int dataWidth) : Component() {
    SetComponentName("if_lt_op_s");
    SetDataWidth(dataWidth);
    CreatePorts();
    CreateGenericMaps();
    SetCycleClockCount(0);
}

// Returns the vhdl decl of the component
string Cmp_if_lt_op_s::GetVHDLDeclaration(string nameExtension){
    string decl = "";

    decl += "component " + GetComponentName() + "\n";
    decl += "generic (\n";
    decl += "	w_in1	: integer := 16;\n";
    decl += "	w_in2	: integer := 16;\n";
    decl += "	w_out	: integer := 1\n";
    decl += ");\n";
    decl += "port (\n";
    decl += "	I0	: in	std_logic_vector(w_in1-1 downto 0);\n";
    decl += "	I1	: in	std_logic_vector(w_in2-1 downto 0);\n";
    decl += "	O0	: out	std_logic_vector(0 downto 0)\n";
    decl += ");\n";
    decl += "end component;\n";
    decl += "\n";

    SetDeclared(true);

    return decl;
}

// Instantiates all ports of the component
void Cmp_if_lt_op_s::CreatePorts() {
    AddPort(new Port("I0", GetDataWidth(), "in", "std_logic_vector"));
    PortStrings["I0"] = 0;
    AddPort(new Port("I1", GetDataWidth(), "in", "std_logic_vector"));
    PortStrings["I1"] = 1;
    AddPort(new Port("O0", GetDataWidth(), "out", "std_logic_vector"));
    PortStrings["O0"] = 2;
}

// Instantiates all maps of the component
void Cmp_if_lt_op_s::CreateGenericMaps() {
    AddGenericMap(new GenericMap("w_in1", "integer", to_string(GetDataWidth())));
    MapStrings["w_in1"] = 0;
    AddGenericMap(new GenericMap("w_in2", "integer", to_string(GetDataWidth())));
    MapStrings["w_in2"] = 1;
    AddGenericMap(new GenericMap("w_out", "integer", to_string(GetDataWidth())));
    MapStrings["w_out"] = 2;
}

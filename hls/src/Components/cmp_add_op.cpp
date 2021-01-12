#include<string>
#include<Core/Component.hpp>
#include<Components/cmp_add_op.hpp>

using namespace std;

Cmp_add_op::Cmp_add_op(int dataWidth) : Component() {
	SetComponentName("add_op");
	SetDataWidth(dataWidth);
    CreatePorts();
    CreateGenericMaps();
}

string Cmp_add_op::GetVHDLDeclaration() {
	string decl = "";

	decl += "component " + GetComponentName() + "\n";
	decl += "generic (\n";
	decl += "	w_in1	: integer := 8;\n";
	decl += "	w_in2	: integer := 8;\n";
	decl += "	w_out	: integer := 16\n";
	decl += ");\n";
	decl += "port (\n";
	decl += "	I0	: in	std_logic_vector(w_in1-1 downto 0);\n";
	decl += "	I1	: in	std_logic_vector(w_in2-1 downto 0);\n";
	decl += "	O0	: out	std_logic_vector(w_out-1 downto 0)\n";
	decl += ");\n";
	decl += "end component;\n";
	decl += "\n";

	SetDeclared(true);

	return decl;
}

void Cmp_add_op::CreatePorts() {
    AddPort(new Port("I0", GetDataWidth(), "in", "std_logic_vector"));
    PortStrings["I0"] = 0;
    AddPort(new Port("I1", GetDataWidth(), "in", "std_logic_vector"));
    PortStrings["I1"] = 1;
    AddPort(new Port("O0", GetDataWidth(), "out", "std_logic_vector"));
    PortStrings["O0"] = 2;
}

// Instantiates all maps of the component
void Cmp_add_op::CreateGenericMaps() {
    AddGenericMap(new GenericMap("w_in1", "integer", to_string(GetDataWidth())));
    MapStrings["w_in1"] = 0;
    AddGenericMap(new GenericMap("w_in2", "integer", to_string(GetDataWidth())));
    MapStrings["w_in2"] = 1;
    AddGenericMap(new GenericMap("w_out", "integer", to_string(GetDataWidth() * 2)));
    MapStrings["w_out"] = 2;
}

#include<string>
#include<Core/Component.hpp>
#include<Components/cmp_add_op_fl.hpp>

using namespace std;

/**
 * The ADD operation for Floating-Point (32-Bits IEEE 754).
 */
Cmp_add_op_fl::Cmp_add_op_fl(int dataWidth) : Component() {
	SetComponentName("add_op_fl");
	SetDataWidth(dataWidth);
    CreatePorts();
    CreateGenericMaps();
}

string Cmp_add_op_fl::GetVHDLDeclaration() {
	string d;
	d += "component " + GetComponentName() + "\n";
	d += "generic (\n";
	d += "	w_in1	: integer := 32;\n";
	d += "	w_in2	: integer := 32;\n";
	d += "	w_out	: integer := 32\n";
	d += ");\n";
	d += "port (\n";
	d += "	I0	: in	std_logic_vector(w_in1-1 downto 0);\n";
	d += "	I1	: in	std_logic_vector(w_in2-1 downto 0);\n";
	d += "	O0	: out	std_logic_vector(w_out-1 downto 0)\n";
	d += ");\n";
	d += "end component;\n";
	d += "\n";
	return d;
}

void Cmp_add_op_fl::CreatePorts() {
    AddPort(new Port("I0", GetDataWidth(), "in", "std_logic_vector"));
    PortStrings["I0"] = 0;
    AddPort(new Port("I1", GetDataWidth(), "in", "std_logic_vector"));
    PortStrings["I1"] = 1;
    AddPort(new Port("O0", GetDataWidth(), "out", "std_logic_vector"));
    PortStrings["O0"] = 2;
}

// Instantiates all maps of the component
void Cmp_add_op_fl::CreateGenericMaps() {
    AddGenericMap(new GenericMap("w_in1", "integer", to_string(GetDataWidth())));
    MapStrings["w_in1"] = 0;
    AddGenericMap(new GenericMap("w_in2", "integer", to_string(GetDataWidth())));
    MapStrings["w_in2"] = 1;
    AddGenericMap(new GenericMap("w_out", "integer", to_string(GetDataWidth() * 2)));
    MapStrings["w_out"] = 2;
}

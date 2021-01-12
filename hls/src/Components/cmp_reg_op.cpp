#include<string>
#include<Core/Component.hpp>
#include<Components/cmp_reg_op.hpp>

Cmp_reg_op::Cmp_reg_op(int dataWidth) : Component() {
    SetComponentName("reg_op");
    SetDataWidth(dataWidth);
    CreatePorts();
    CreateGenericMaps();
    SetCycleClockCount(1);
}

// Returns the vhdl decl of the component
string Cmp_reg_op::GetVHDLDeclaration(string nameExtension){
    string decl = "";

    decl += "component " + GetComponentName() + "\n";
    decl += "generic (\n";
    decl += "	w_in	: integer := 16;\n";
    decl += "	initial	: integer := 0\n";
    decl += ");\n";
    decl += "port (\n";
	decl += "        clk      : in	std_logic;\n";
	decl += "        reset    : in	std_logic;\n";
	decl += "        we       : in    std_logic := '1';\n";
	decl += "        I0       : in    std_logic_vector(w_in-1 downto 0);\n";
	decl += "        O0       : out   std_logic_vector(w_in-1 downto 0)\n";
    decl += ");\n";
    decl += "end component;\n";
    decl += "\n";

    SetDeclared(true);

    return decl;
}

// Instantiates all ports of the component
void Cmp_reg_op::CreatePorts() {
    AddPort(new Port("clk", 1, "in", "std_logic"));
    PortStrings["clk"] = 0;
    AddPort(new Port("reset", 1, "in", "std_logic"));
    PortStrings["reset"] = 1;
    AddPort(new Port("we", 1, "in", "std_logic"));
    PortStrings["we"] = 2;
    AddPort(new Port("I0", GetDataWidth(), "in", "std_logic_vector"));
    PortStrings["I0"] = 3;
    AddPort(new Port("O0", GetDataWidth(), "out", "std_logic_vector"));
    PortStrings["O0"] = 4;

}

// Instantiates all maps of the component
void Cmp_reg_op::CreateGenericMaps() {
    AddGenericMap(new GenericMap("w_in", "integer", to_string(GetDataWidth())));
    MapStrings["w_in"] = 0;
    AddGenericMap(new GenericMap("initial", "integer", "0"));
    MapStrings["initial"] = 1;

}

#include<string>
#include<Core/Component.hpp>
#include<Components/cmp_add_reg_op_s.hpp>

Cmp_add_reg_op_s::Cmp_add_reg_op_s(int dataWidth) : Component() {
    SetComponentName("add_reg_op_s");
    SetDataWidth(dataWidth);
    CreatePorts();
    CreateGenericMaps();
    SetCycleClockCount(1);
}

// Returns the vhdl decl of the component
string Cmp_add_reg_op_s::GetVHDLDeclaration(string nameExtension){
    string decl = "";

    decl += "component " + GetComponentName() + "\n";
    decl += "generic ( \n";
    decl += "        w_in1	: integer := 16; \n";
    decl += "        w_in2	: integer := 16; \n";
    decl += "        w_out	: integer := 32; \n";
    decl += "        initial	: integer := 0\n";
    decl += "); \n";
    decl += "port ( \n";
    decl += "        clk         : in	std_logic; \n";
    decl += "        reset       : in	std_logic; \n";
    decl += "        we          : in	std_logic := '1'; \n";
    decl += "        Sel1        : in	std_logic_vector(0 downto 0) := \"1\"; \n";
    decl += "        I0          : in	std_logic_vector(w_in1-1 downto 0); \n";
    decl += "        I1          : in	std_logic_vector(w_in2-1 downto 0); \n";
    decl += "        O0          : out	std_logic_vector(w_out-1 downto 0) \n";
    decl += "); \n";
    decl += "end component; \n\n";

    SetDeclared(true);

    return decl;
}

// Instantiates all ports of the component
void Cmp_add_reg_op_s::CreatePorts() {
    AddPort(new Port("clk", 1, "in", "std_logic"));
    PortStrings["clk"] = 0;
    AddPort(new Port("reset", 1, "in", "std_logic"));
    PortStrings["reset"] = 1;
    AddPort(new Port("we", 1, "in", "std_logic"));
    PortStrings["we"] = 2;
    AddPort(new Port("Sel1", 1, "in", "std_logic_vector"));
    PortStrings["Sel1"] = 3;
    AddPort(new Port("I0", GetDataWidth(), "in", "std_logic_vector"));
    PortStrings["I0"] = 4;
    AddPort(new Port("I1", GetDataWidth(), "in", "std_logic_vector"));
    PortStrings["I1"] = 5;
    AddPort(new Port("O0", GetDataWidth(), "out", "std_logic_vector"));
    PortStrings["O0"] = 6;
}

// Instantiates all maps of the component
void Cmp_add_reg_op_s::CreateGenericMaps() {
    AddGenericMap(new GenericMap("initial", "integer", "0"));
    MapStrings["initial"] = 0;
    AddGenericMap(new GenericMap("w_in1", "integer", to_string(GetDataWidth())));
    MapStrings["w_in1"] = 1;
    AddGenericMap(new GenericMap("w_in2", "integer", to_string(GetDataWidth())));
    MapStrings["w_in2"] = 2;
    AddGenericMap(new GenericMap("w_out", "integer", to_string(GetDataWidth())));
    MapStrings["w_out"] = 3;
}

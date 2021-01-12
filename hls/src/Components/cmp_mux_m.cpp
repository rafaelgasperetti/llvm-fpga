#include<string>
#include<Core/Component.hpp>
#include<Components/cmp_mux_m.hpp>

Cmp_mux_m::Cmp_mux_m(int dataWidth) : Component() {
    SetComponentName("mux_m");
    SetDataWidth(dataWidth);
    CreatePorts();
    CreateGenericMaps();
    SetCycleClockCount(0);
}

// Returns the vhdl decl of the component
string Cmp_mux_m::GetVHDLDeclaration(string nameExtension){
    string decl = "";

    decl += "component " + GetComponentName() + "\n";
    decl += "generic (\n";
    decl += "	w_in	: integer := 32;\n";
    decl += "	N_ops	: integer := 32;\n";
    decl += "	N_sels	: integer := 31\n";
    decl += ");\n";
    decl += "port (\n";
    decl += "	I0	: in	std_logic_vector((w_in*N_ops)-1 downto 0);\n";
    decl += "	Sel	: in	std_logic_vector(N_sels-1 downto 0);\n";
    decl += "	O0	: out	std_logic_vector(w_in-1 downto 0)\n";
    decl += ");\n";
    decl += "end component;\n";
    decl += "\n";

    SetDeclared(true);

    return decl;
}

// Instantiates all ports of the component
void Cmp_mux_m::CreatePorts() {
    AddPort(new Port("I0", GetDataWidth(), "in", "std_logic_vector"));
    PortStrings["I0"] = 0;
    AddPort(new Port("Sel", GetDataWidth(), "in", "std_logic_vector"));
    PortStrings["Sel"] = 1;
    AddPort(new Port("O0", GetDataWidth(), "out", "std_logic_vector"));
    PortStrings["O0"] = 2;
}

// Instantiates all maps of the component
void Cmp_mux_m::CreateGenericMaps() {
    AddGenericMap(new GenericMap("w_in", "integer", to_string(GetDataWidth())));
    MapStrings["w_in"] = 0;
    AddGenericMap(new GenericMap("N_ops", "integer", to_string(GetDataWidth())));
    MapStrings["N_ops"] = 1;
    AddGenericMap(new GenericMap("N_sels", "integer", to_string(GetDataWidth())));
    MapStrings["N_sels"] = 2;
}

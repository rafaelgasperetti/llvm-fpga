#include<string>
#include<Core/Component.hpp>
#include<Components/cmp_delay_op.hpp>

Cmp_delay_op::Cmp_delay_op(int dataWidth) : Component() {
	SetComponentName("delay_op");
	SetDataWidth(dataWidth);
	CreatePorts();
	CreateGenericMaps();
	SetCycleClockCount(-1);
}

// Returns the vhdl decl of the component
string Cmp_delay_op::GetVHDLDeclaration(string nameExtension) {
	string decl = "";

	decl += "component " + GetComponentName() + " \n";
	decl += "generic ( \n";
	decl += "        bits        : integer := 8; \n";
	decl += "        delay       : integer := 1 \n";
	decl += "); \n";
	decl += "port ( \n";
	decl += "        a		: in	std_logic_vector(bits-1 downto 0); \n";
	decl += "        clk		: in	std_logic; \n";
	decl += "        reset	: in	std_logic; \n";
	decl += "        a_delayed	: out	std_logic_vector(bits-1 downto 0) := (others=>'0') \n";
	decl += "); \n";
	decl += "end component; \n\n";

	SetDeclared(true);

	return decl;
}

// Instantiates all ports of the component
void Cmp_delay_op::CreatePorts() {
	AddPort(new Port("a", 0, "in", "std_logic_vector"));
	PortStrings["a"] = 0;
	AddPort(new Port("clk", 0, "in", "std_logic_vector"));
	PortStrings["clk"] = 1;
	AddPort(new Port("reset", 0, "in", "std_logic"));
	PortStrings["reset"] = 2;
	AddPort(new Port("a_delayed", 0, "out", "std_logic_vector"));
	PortStrings["a_delayed"] = 3;
}

// Instantiates all maps of the component
void Cmp_delay_op::CreateGenericMaps() {
	AddGenericMap(new GenericMap("bits", "integer", "32"));
	MapStrings["bits"] = 0;
	AddGenericMap(new GenericMap("delay", "integer", "0"));
	MapStrings["delay"] = 1;
}

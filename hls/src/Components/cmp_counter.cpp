#include<string>
#include<Core/Component.hpp>
#include<Components/cmp_counter.hpp>

Cmp_counter::Cmp_counter(int dataWidth) : IOComponent() {
	SetComponentName("counter");
	SetDataWidth(dataWidth);
	CreatePorts();
	CreateGenericMaps();
	SetCycleClockCount(-1);
}

// Returns the vhdl decl of the component
string Cmp_counter::GetVHDLDeclaration(string nameExtension) {
	string decl = "";

	decl += "component " + GetComponentName() + " \n";
	decl += "generic (\n";
	decl += "        bits		: integer := 8;\n";
	decl += "        steps		: integer := 1;\n";
	decl += "        increment           : integer := 1;\n";
	decl += "        down                : integer := 0;\n";
	decl += "        condition           : integer := 0\n";
	decl += ");\n";
	decl += "port (\n";
	decl += "        input		: in	std_logic_vector(bits-1 downto 0);\n";
	decl += "        termination         : in	std_logic_vector(bits-1 downto 0);\n";
	decl += "        clk                 : in	std_logic;\n";
	decl += "        clk_en		: in	std_logic := '1';\n";
	decl += "        reset		: in	std_logic;\n";
	decl += "        load		: in	std_logic := '0';\n";
	decl += "        step		: out	std_logic;\n";
	decl += "        done		: out	std_logic;\n";
	decl += "        output		: out	std_logic_vector(bits-1 downto 0)\n";
	decl += "); \n";
	decl += "end component; \n\n";

	SetDeclared(true);

	return decl;
}

// Instantiates all ports of the component
void Cmp_counter::CreatePorts() {
	AddPort(new Port("input", GetDataWidth(), "in", "std_logic_vector"));
	PortStrings["input"] = 0;
	AddPort(new Port("termination", GetDataWidth(), "in", "std_logic_vector"));
	PortStrings["termination"] = 1;
	AddPort(new Port("clk", 1, "in", "std_logic"));
	PortStrings["clk"] = 2;
	AddPort(new Port("clk_en", 1, "in", "std_logic"));
	PortStrings["clk_en"] = 3;
	AddPort(new Port("reset", 1, "in", "std_logic"));
	PortStrings["reset"] = 4;
	AddPort(new Port("load", 1, "in", "std_logic"));
	PortStrings["load"] = 5;
	AddPort(new Port("step", 1, "out", "std_logic"));
	PortStrings["step"] = 6;
	AddPort(new Port("done", 1, "out", "std_logic"));
	PortStrings["done"] = 7;
	AddPort(new Port("output", GetDataWidth(), "out", "std_logic_vector"));
	PortStrings["output"] = 8;
}

// Instantiates all maps of the component
void Cmp_counter::CreateGenericMaps() {
	AddGenericMap(new GenericMap("bits", "integer", to_string(GetDataWidth())));
	MapStrings["bits"] = 0;
	AddGenericMap(new GenericMap("steps", "integer", "1"));
	MapStrings["steps"] = 1;
	AddGenericMap(new GenericMap("increment", "integer", "1"));
	MapStrings["increment"] = 2;
	AddGenericMap(new GenericMap("down", "integer", "0"));
	MapStrings["down"] = 3;
	AddGenericMap(new GenericMap("condition", "integer", "0"));
	MapStrings["condition"] = 4;
}

int Cmp_counter::GetConditionValue(string operation)
{
    if(operation.compare(NodeInfo::Operation_LessThan) == 0)
    {
        return 0;
    }
    else if(operation.compare(NodeInfo::Operation_LessEqualThan) == 0)
    {
        return 1;
    }
    else if(operation.compare(NodeInfo::Operation_GreaterThan) == 0)
    {
        return 2;
    }
    else if(operation.compare(NodeInfo::Operation_GreaterEqualThan) == 0)
    {
        return 3;
    }
    else if(operation.compare(NodeInfo::Operation_Equal) == 0)
    {
        return 4;
    }
    else if(operation.compare(NodeInfo::Operation_Different) == 0)
    {
        return 5;
    }
    else
    {
        throw invalid_argument("Unexpected operation '" + operation + ", which is not supported by the counter.");
    }
}

void Cmp_counter::SetIntrementFactor(float incrementFactor)
{
    IncrementFactor = incrementFactor;
}

float Cmp_counter::GetIncrementFactor()
{
    return IncrementFactor;
}

void Cmp_counter::SetSteps(int steps)
{
    Steps = steps;
}

int Cmp_counter::GetSteps()
{
    return Steps;
}

#ifndef CMPBLOCKRAM_H
#define	CMPBLOCKRAM_H

#include<string>

#include<Core/Component.hpp>
#include<Components/cmp_block_ram.hpp>

Cmp_block_ram::Cmp_block_ram(int dataWidth) : Component() {
    BlockRam = true;
	SetComponentName("block_ram");
	SetDataWidth(dataWidth);
	CreatePorts();
	CreateGenericMaps();
	SetCycleClockCount(1);
}

// Returns the vhdl decl of the component
string Cmp_block_ram::GetVHDLDeclaration(string nameExtension){
	string decl = "";

	decl += "component " + GetComponentName() + (nameExtension.compare("") == 0 ? "" : "_" + nameExtension) + " \n";
	decl += "generic ( \n";
	decl += "        data_width          : integer := " + GetMap("data_width")->GetValue() + "; \n";
	decl += "        address_width	: integer := " + GetMap("address_width")->GetValue() + " \n";
	decl += "); \n";
	decl += "port ( \n";
	decl += "        clk                 : in	std_logic; \n";
	decl += "        we                  : in	std_logic := '0'; \n";
	decl += "        oe                  : in	std_logic := '1'; \n";
	decl += "        address             : in	std_logic_vector(address_width-1 downto 0); \n";
	decl += "        data_in             : in	std_logic_vector(data_width-1 downto 0) := (others => '0'); \n";
	decl += "        data_out            : out	std_logic_vector(data_width-1 downto 0) \n";
	decl += "); \n";
	decl += "end component; \n\n";

	SetDeclared(true);

	return decl;
}

// Instantiates all ports of the component
void Cmp_block_ram::CreatePorts() {
	AddPort(new Port("clk", 1, "in", "std_logic"));
	PortStrings["clk"] = 0;
	AddPort(new Port("oe", 1, "in", "std_logic"));
	PortStrings["ow"] = 1;
	AddPort(new Port("we", 1, "in", "std_logic"));
	PortStrings["we"] = 2;
	AddPort(new Port("address", 8, "in", "std_logic_vector"));
	PortStrings["address"] = 3;
	AddPort(new Port("data_in", GetDataWidth(), "in", "std_logic_vector"));
	PortStrings["data_in"] = 4;
	AddPort(new Port("data_out", GetDataWidth(), "out", "std_logic_vector"));
	PortStrings["data_out"] = 5;
}

// Instantiates all maps of the component
void Cmp_block_ram::CreateGenericMaps() {
	AddGenericMap(new GenericMap("data_width", "integer", to_string(GetDataWidth())));
	MapStrings["data_width"] = 0;
	AddGenericMap(new GenericMap("address_width", "integer", "8"));
	MapStrings["address_width"] = 1;
}

void Cmp_block_ram::SetMaxSize(int maxSize)
{
    if(maxSize < 0)
    {
        MaxSize = 0;
    }
    else
    {
        MaxSize = maxSize;
    }
    GetMap("address_width")->SetValue(to_string(Component::GetBinaryValue(maxSize, 0).size()));
    GetPort("address")->SetWidth(Component::GetBinaryValue(maxSize, 0).size());
    GetPort("address")->SetUsedWidthFinal("0");
    GetPort("address")->SetUsedWidthInitial(to_string(GetPort("address")->GetWidth() - 1));
}

int Cmp_block_ram::GetMaxSize()
{
    return MaxSize;
}

string Cmp_block_ram::GetEntityDeclaration(Cmp_block_ram* ram, int number)
{
    string decl = "";

    decl += "entity " + ram->GetComponentName() + "_" + to_string(number) + " is\n";
    decl += "generic(\n";
    decl += "   data_width : integer := " + ram->GetMap("data_width")->GetValue() + ";\n";
    decl += "   address_width : integer := " + ram->GetMap("address_width")->GetValue() + "\n";
    decl += ");\n";
    decl += "port(\n";
    decl += "   data_in : in std_logic_vector(data_width-1 downto 0) := (others => '0');\n";
    decl += "   address : in std_logic_vector(address_width-1 downto 0);\n";
    decl += "   we: in std_logic := '0';\n";
    decl += "   oe: in std_logic := '1';\n";
    decl += "   clk : in std_logic;\n";
    decl += "   data_out : out std_logic_vector(data_width-1 downto 0));\n";
    decl += "end " + ram->GetComponentName() + "_" + to_string(number) + ";\n\n";

    decl += "architecture rtl of " + ram->GetComponentName() + "_" + to_string(number) + " is\n\n";

    decl += "constant mem_depth : integer := 2**address_width;\n";
    decl += "type ram_type is array (mem_depth-1 downto 0)\n";
    decl += "of std_logic_vector (data_width-1 downto 0);";

    decl += "signal read_a : std_logic_vector(address_width-1 downto 0);\n";

    decl += "signal RAM : ram_type := ram_type(\n";

    for(int i = 0; i < ram->GetMaxSize(); i++)
    {
        if(i == stoi(ram->GetMap("address_width")->GetValue()) - 1)
        {
            decl += "\t(\"" + StringUtils::Replicate("0", stoi(ram->GetMap("data_width")->GetValue())) + "\") --" + to_string(i) + "\n";
        }
        else
        {
            decl += "\t(\"" + StringUtils::Replicate("0", stoi(ram->GetMap("data_width")->GetValue())) + "\"), --" + to_string(i) + "\n";
        }
    }

    decl += ");\n";

    decl += "begin\n";
    decl += "   process(clk)\n";
    decl += "   begin\n";
    decl += "       if(clk'event and clk = '1') then\n";
    decl += "           if (we = '1') then\n";
    decl += "               RAM(conv_integer(address)) <= data_in;\n";
    decl += "               data_out <= RAM(conv_integer(read_a));\n";
    decl += "           elsif(oe = '1') then\n";
    decl += "               data_out <= RAM(conv_integer(read_a));\n";
    decl += "           end if;\n";
    decl += "           read_a <= address;\n";
    decl += "       end if;\n";
    decl += "   end process;\n";
    decl += "end rtl;";

    return decl;
}

#endif

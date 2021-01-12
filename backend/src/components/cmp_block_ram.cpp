#include <string>
#include "core/component.hpp"
#include "components/cmp_block_ram.hpp"

bool Cmp_block_ram::isDeclared = false;

Cmp_block_ram::Cmp_block_ram(int dataWidth) : Component() {
	this->setComponentName("block_ram");
	this->setDataWidth(dataWidth);
	this->createPorts();
	this->createGenericMaps();
}

void Cmp_block_ram::setIsDeclared(bool isDeclared) {
	this->isDeclared = isDeclared;
}

bool Cmp_block_ram::getIsDeclared() {
	return isDeclared;
}

// Returns the vhdl decl of the component
std::string Cmp_block_ram::getVHDLDeclaration(std::string nameExtension){
	std::string decl = "";

	decl += "component " + this->getComponentName() + nameExtension + " \n";
	decl += "generic ( \n";
	decl += "        data_width          : integer := 8; \n";
	decl += "        address_width	: integer := 8 \n";
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

	this->setIsDeclared(true);

	return decl;
}

// Instantiates all ports of the component
void Cmp_block_ram::createPorts() {
	this->addPort(new Port("clk", "in", "std_logic", "1"));
	this->portStrings["clk"] = 0;
	this->addPort(new Port("oe", "in", "std_logic", "1"));
	this->portStrings["ow"] = 1;
	this->addPort(new Port("we", "in", "std_logic", "1"));
	this->portStrings["we"] = 2;
	this->addPort(new Port("address", "in", "std_logic_vector", "8"));
	this->portStrings["address"] = 3;
	this->addPort(new Port("data_in", "in", "std_logic_vector", std::to_string(getDataWidth())));
	this->portStrings["data_in"] = 4;
	this->addPort(new Port("data_out", "out", "std_logic_vector", std::to_string(getDataWidth())));
	this->portStrings["data_out"] = 5;
}

// Instantiates all maps of the component
void Cmp_block_ram::createGenericMaps() {
	this->addGenericMap(new GenericMap("data_width", "integer", std::to_string(getDataWidth())));
	this->mapStrings["data_width"] = 0;	
	this->addGenericMap(new GenericMap("address_width", "integer", "8"));
	this->mapStrings["address_width"] = 1;
}

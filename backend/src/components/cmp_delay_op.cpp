#include <string>
#include "core/component.hpp"
#include "components/cmp_delay_op.hpp"

bool Cmp_delay_op::isDeclared = false;

Cmp_delay_op::Cmp_delay_op(int dataWidth) : Component() {
	this->setComponentName("delay_op");
	this->setDataWidth(dataWidth);
	this->createPorts();
	this->createGenericMaps();
}

void Cmp_delay_op::setIsDeclared(bool isDeclared) {
	this->isDeclared = isDeclared;
}

bool Cmp_delay_op::getIsDeclared() {
	return isDeclared;
}

// Returns the vhdl decl of the component
std::string Cmp_delay_op::getVHDLDeclaration(std::string nameExtension) {
	std::string decl = "";

	decl += "component " + this->getComponentName() + " \n";
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

	this->setIsDeclared(true);

	return decl;
}

// Instantiates all ports of the component
void Cmp_delay_op::createPorts() {
	this->addPort(new Port("a", "in", "std_logic_vector", "0"));
	this->portStrings["a"] = 0;
	this->addPort(new Port("clk", "in", "std_logic_vector", "0"));
	this->portStrings["clk"] = 1;
	this->addPort(new Port("reset", "in", "std_logic", "0"));
	this->portStrings["reset"] = 2;
	this->addPort(new Port("a_delayed", "out", "std_logic_vector", "0"));
	this->portStrings["a_delayed"] = 3;
}

// Instantiates all maps of the component
void Cmp_delay_op::createGenericMaps() {
	this->addGenericMap(new GenericMap("bits", "integer", "32"));
	this->mapStrings["bits"] = 0;	
	this->addGenericMap(new GenericMap("delay", "integer", "0"));
	this->mapStrings["delay"] = 1;
}

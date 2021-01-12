#include <string>
#include "core/component.hpp"
#include "components/cmp_counter.hpp"

bool Cmp_counter::isDeclared = false;

Cmp_counter::Cmp_counter(int dataWidth) : Component() {
	this->setComponentName("counter");
	this->setDataWidth(dataWidth);
	this->createPorts();
	this->createGenericMaps();
}

void Cmp_counter::setIsDeclared(bool isDeclared) {
	this->isDeclared = isDeclared;
}

bool Cmp_counter::getIsDeclared() {
	return isDeclared;
}

// Returns the vhdl decl of the component
std::string Cmp_counter::getVHDLDeclaration(std::string nameExtension) {
	std::string decl = "";

	decl += "component " + this->getComponentName() + " \n";
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

	this->setIsDeclared(true);

	return decl;
}

// Instantiates all ports of the component
void Cmp_counter::createPorts() {
	this->addPort(new Port("input", "in", "std_logic_vector", std::to_string(getDataWidth())));
	this->portStrings["input"] = 0;
	this->addPort(new Port("termination", "in", "std_logic_vector", std::to_string(getDataWidth())));
	this->portStrings["termination"] = 1;
	this->addPort(new Port("clk", "in", "std_logic", "1"));
	this->portStrings["clk"] = 2;
	this->addPort(new Port("clk_en", "in", "std_logic", "1"));
	this->portStrings["clk_en"] = 3;
	this->addPort(new Port("reset", "in", "std_logic", "1"));
	this->portStrings["reset"] = 4;
	this->addPort(new Port("load", "in", "std_logic", "1"));
	this->portStrings["load"] = 5;
	this->addPort(new Port("step", "out", "std_logic", "1"));
	this->portStrings["step"] = 6;
	this->addPort(new Port("done", "out", "std_logic", "1"));
	this->portStrings["done"] = 7;
	this->addPort(new Port("output", "out", "std_logic_vector", std::to_string(getDataWidth())));
	this->portStrings["output"] = 8;
}

// Instantiates all maps of the component
void Cmp_counter::createGenericMaps() {
	this->addGenericMap(new GenericMap("bits", "integer", std::to_string(getDataWidth())));
	this->mapStrings["bits"] = 0;
	this->addGenericMap(new GenericMap("steps", "integer", "1"));
	this->mapStrings["steps"] = 1;
	this->addGenericMap(new GenericMap("increment", "integer", "1"));
	this->mapStrings["increment"] = 2;
	this->addGenericMap(new GenericMap("down", "integer", "0"));
	this->mapStrings["down"] = 3;
	this->addGenericMap(new GenericMap("condition", "integer", "0"));
	this->mapStrings["condition"] = 4;
}

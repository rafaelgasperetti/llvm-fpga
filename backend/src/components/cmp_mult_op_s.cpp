#include <string>
#include "core/component.hpp"
#include "components/cmp_mult_op_s.hpp"

bool Cmp_mult_op_s::isDeclared = false;

Cmp_mult_op_s::Cmp_mult_op_s(int dataWidth) : Component() {
	this->setComponentName("mult_op_s");
	this->setDataWidth(dataWidth);
	this->createPorts();
	this->createGenericMaps();
}

void Cmp_mult_op_s::setIsDeclared(bool isDeclared) {
	this->isDeclared = isDeclared;
}

bool Cmp_mult_op_s::getIsDeclared() {
	return isDeclared;
}

// Returns the vhdl decl of the component
std::string Cmp_mult_op_s::getVHDLDeclaration(std::string nameExtension) {
	std::string decl = "";

	decl += "component " + this->getComponentName() + " \n";
	decl += "generic (\n";
	decl += "        w_in1	: integer := 8;\n";
	decl += "        w_in2	: integer := 8;\n";
	decl += "        w_out	: integer := 16\n";
	decl += ");\n";
	decl += "port (\n";
	decl += "        I0          : in	std_logic_vector(w_in1-1 downto 0);\n";
	decl += "        I1          : in	std_logic_vector(w_in2-1 downto 0);\n";
	decl += "        O0          : out	std_logic_vector(w_out-1 downto 0)\n";
	decl += "); \n";
	decl += "end component; \n\n";

	this->setIsDeclared(true);

	return decl;
}

// Instantiates all ports of the component
void Cmp_mult_op_s::createPorts() {   
    this->addPort(new Port("I0", "in", "std_logic_vector", std::to_string(getDataWidth())));
    this->portStrings["I0"] = 0;
    this->addPort(new Port("I1", "in", "std_logic_vector", std::to_string(getDataWidth())));
    this->portStrings["I1"] = 1;
    this->addPort(new Port("O0", "out", "std_logic_vector", std::to_string(getDataWidth())));
    this->portStrings["O0"] = 2;
}

// Instantiates all maps of the component
void Cmp_mult_op_s::createGenericMaps() {
    this->addGenericMap(new GenericMap("w_in1", "integer", "8"));
    this->mapStrings["w_in1"] = 0;
    this->addGenericMap(new GenericMap("w_in2", "integer", "8"));
    this->mapStrings["w_in2"] = 1;
    this->addGenericMap(new GenericMap("w_out", "integer", "16"));
    this->mapStrings["w_out"] = 2;
}


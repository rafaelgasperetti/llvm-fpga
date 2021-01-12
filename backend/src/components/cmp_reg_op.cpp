#include <string>
#include "core/component.hpp"
#include "components/cmp_reg_op.hpp"

bool Cmp_reg_op::isDeclared = false;

Cmp_reg_op::Cmp_reg_op(int dataWidth) : Component() {
    this->setComponentName("reg_op");
    this->setDataWidth(dataWidth);
    this->createPorts();
    this->createGenericMaps();
}

void Cmp_reg_op::setIsDeclared(bool isDeclared) {
    this->isDeclared = isDeclared;
}

bool Cmp_reg_op::getIsDeclared() {
    return isDeclared;
}

// Returns the vhdl decl of the component
std::string Cmp_reg_op::getVHDLDeclaration(std::string nameExtension){
    std::string decl = "";
    
    decl += "component reg_op\n";  
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
    
    return decl;

    this->setIsDeclared(true);

    return decl;
}

// Instantiates all ports of the component
void Cmp_reg_op::createPorts() {   
    this->addPort(new Port("clk", "in", "std_logic", std::to_string(getDataWidth())));
    this->portStrings["clk"] = 0;
    this->addPort(new Port("reset", "in", "std_logic", std::to_string(getDataWidth())));
    this->portStrings["reset"] = 1;
    this->addPort(new Port("we", "in", "std_logic", std::to_string(getDataWidth())));
    this->portStrings["we"] = 2;
    this->addPort(new Port("I0", "in", "std_logic_vector", std::to_string(getDataWidth())));
    this->portStrings["I0"] = 3;
    this->addPort(new Port("O0", "out", "std_logic_vector", std::to_string(getDataWidth())));
    this->portStrings["O0"] = 4;
    
}

// Instantiates all maps of the component
void Cmp_reg_op::createGenericMaps() {
    this->addGenericMap(new GenericMap("w_in", "integer", std::to_string(getDataWidth())));
    this->mapStrings["w_in"] = 0;    
    this->addGenericMap(new GenericMap("initial", "integer", std::to_string(getDataWidth())));
    this->mapStrings["initial"] = 0;

}

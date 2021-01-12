#include <string>
#include "core/component.hpp"
#include "components/cmp_add_reg_op_s.hpp"

bool Cmp_add_reg_op_s::isDeclared = false;

Cmp_add_reg_op_s::Cmp_add_reg_op_s(int dataWidth) : Component() {
    this->setComponentName("add_reg_op_s");
    this->setDataWidth(dataWidth);
    this->createPorts();
    this->createGenericMaps();
}

void Cmp_add_reg_op_s::setIsDeclared(bool isDeclared) {
    this->isDeclared = isDeclared;
}

bool Cmp_add_reg_op_s::getIsDeclared() {
    return isDeclared;
}

// Returns the vhdl decl of the component
std::string Cmp_add_reg_op_s::getVHDLDeclaration(std::string nameExtension){
    std::string decl = "";
    
    decl += "component " + this->getComponentName() + "\n";
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

    this->setIsDeclared(true);

    return decl;
}

// Instantiates all ports of the component
void Cmp_add_reg_op_s::createPorts() {   
    this->addPort(new Port("clk", "in", "std_logic", "1"));
    this->portStrings["clk"] = 0;
    this->addPort(new Port("reset", "in", "std_logic", "1"));
    this->portStrings["reset"] = 1;
    this->addPort(new Port("we", "in", "std_logic", "1"));
    this->portStrings["we"] = 2;
    this->addPort(new Port("Sel1", "in", "std_logic_vector", "1"));
    this->portStrings["Sel1"] = 3;
    this->addPort(new Port("I0", "in", "std_logic_vector", std::to_string(getDataWidth())));
    this->portStrings["I0"] = 4;
    this->addPort(new Port("I1", "in", "std_logic_vector", std::to_string(getDataWidth())));
    this->portStrings["I1"] = 5;
    this->addPort(new Port("O0", "out", "std_logic_vector", std::to_string(getDataWidth())));
    this->portStrings["O0"] = 6;
}

// Instantiates all maps of the component
void Cmp_add_reg_op_s::createGenericMaps() {
    this->addGenericMap(new GenericMap("initial", "integer", "0"));
    this->mapStrings["initial"] = 0;
    this->addGenericMap(new GenericMap("w_in1", "integer", std::to_string(getDataWidth())));
    this->mapStrings["w_in1"] = 1;
    this->addGenericMap(new GenericMap("w_in2", "integer", std::to_string(getDataWidth())));
    this->mapStrings["w_in2"] = 2;
    this->addGenericMap(new GenericMap("w_out", "integer", std::to_string(getDataWidth())));
    this->mapStrings["w_out"] = 3;
}

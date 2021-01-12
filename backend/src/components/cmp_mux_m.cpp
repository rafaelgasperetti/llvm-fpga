#include <string>
#include "core/component.hpp"
#include "components/cmp_mux_m.hpp"

bool Cmp_mux_m::isDeclared = false;

Cmp_mux_m::Cmp_mux_m(int dataWidth) : Component() {
    this->setComponentName("mux_m");
    this->setDataWidth(dataWidth);
    this->createPorts();
    this->createGenericMaps();
}

void Cmp_mux_m::setIsDeclared(bool isDeclared) {
    this->isDeclared = isDeclared;
}

bool Cmp_mux_m::getIsDeclared() {
    return isDeclared;
}

// Returns the vhdl decl of the component
std::string Cmp_mux_m::getVHDLDeclaration(std::string nameExtension){
    std::string decl = "";
    
    decl += "component mux_m\n";  
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
    
    return decl;

    this->setIsDeclared(true);

    return decl;
}

// Instantiates all ports of the component
void Cmp_mux_m::createPorts() {   
    this->addPort(new Port("I0", "in", "std_logic_vector", std::to_string(getDataWidth())));
    this->portStrings["I0"] = 0;
    this->addPort(new Port("Sel", "in", "std_logic_vector", std::to_string(getDataWidth())));
    this->portStrings["Sel"] = 1;
    this->addPort(new Port("O0", "out", "std_logic_vector", std::to_string(getDataWidth())));
    this->portStrings["O0"] = 2;
}

// Instantiates all maps of the component
void Cmp_mux_m::createGenericMaps() {
    this->addGenericMap(new GenericMap("w_in", "integer", std::to_string(getDataWidth())));
    this->mapStrings["w_in"] = 0;
    this->addGenericMap(new GenericMap("N_ops", "integer", std::to_string(getDataWidth())));
    this->mapStrings["N_ops"] = 1;
    this->addGenericMap(new GenericMap("N_sels", "integer", std::to_string(getDataWidth())));
    this->mapStrings["N_sels"] = 2;
}

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "algorithms/vhdlGen.hpp"
#include "core/component.hpp"
#include "core/port.hpp"
#include "core/signal.hpp"
#include "core/genericMap.hpp"

// Global static pointer
VHDLGen* VHDLGen::m_pInstance = NULL;

// Calls the private constructor
VHDLGen* VHDLGen::Instance()
{
		if (!m_pInstance)   // If a class dont exists yet
			m_pInstance = new VHDLGen;
		return m_pInstance;
}

// Returns the vhdl declaration of libraries
// TODO(?): Implement a method to get the dependencies of each program
std::string VHDLGen::getLibrariesDeclaration() {
	std::string libl = "";

	libl += "library IEEE;\n";
	libl += "use IEEE.std_logic_1164.all;\n";
	libl += "use IEEE.std_logic_arith.all;\n";
	libl += "use IEEE.std_logic_unsigned.all;\n";

	return libl;
}

// Returns the VHDL declaration of the program entity
// All programs will have these same parameters
std::string VHDLGen::getEntityDeclaration(std::string entityName) {
	std::string entity = "";

	entity += "entity " + entityName +" is\n";
	entity += "	port(\n";
	entity += "		\\init\\	: in	std_logic;\n";
	entity += "		\\done\\	: out	std_logic;\n";
	entity += "		\\result\\	: out	std_logic_vector(31 downto 0);\n";
	entity += "		\\clk\\		: in	std_logic;\n";
	entity += "		\\reset\\	: in	std_logic;\n";
	entity += "		\\clear\\	: in	std_logic\n";
	entity += "	); \n";
	entity += "end " + entityName + ";\n\n";

	return entity;
}

std::string VHDLGen::getSignalsDeclaration(std::vector<Signal*> signals) {
	std::string signalsDecl = "";
	
	for (int i = 0; i < signals.size(); i++) {
		if(signals[i]->getSignalName() == "\\init\\" ||
			signals[i]->getSignalName() == "\\done\\" ||
			signals[i]->getSignalName() == "\\result\\" ||
			signals[i]->getSignalName() == "\\clk\\" ||
			signals[i]->getSignalName() == "\\reset\\" ||
			signals[i]->getSignalName() == "\\clear\\" )
			continue;

		signalsDecl += "signal " + signals[i]->getSignalName() + "	: " + signals[i]->getType();
		if (signals[i]->getType() == "std_logic_vector") {
			if (signals[i]->getDataWidth() == 1)
				signalsDecl += "(0 downto 0);\n";
			else
				signalsDecl += "(" + std::to_string(signals[i]->getDataWidth() - 1) +
				" downto 0);\n";
		}
		else {
			signalsDecl += ";\n";
		}
	}

	signalsDecl += "\n";

	return signalsDecl;
}

std::string VHDLGen::getComponentInstancesDeclaration(std::vector<Component*> components) {
	std::string componentDecl = "";

	for (int i = 0; i < components.size(); i++) {
		componentDecl += components[i]->getInstanceName() + ": " + components[i]->getComponentName() +
			             "\n";
		componentDecl += VHDLGen::getComponentMapsDeclaration(components[i]->getMaps());
		componentDecl += VHDLGen::getComponentPortsDeclaration(components[i]->getPorts());
		componentDecl += "\n";
	}
	return componentDecl;
}

std::string VHDLGen::getComponentMapsDeclaration(std::vector<GenericMap*> maps) {
	std::string mapsDecl = "";

	mapsDecl += "generic map (\n";
	for (int i = 0; i < maps.size(); i++) {
		mapsDecl += "	" + maps[i]->getName() + " => " + maps[i]->getValue();
		
		// If its the last decl it doesnt hava a , in the end
		if(i == (maps.size() - 1))
			mapsDecl += "\n";
		else
			mapsDecl += ",\n";  
	}
	mapsDecl += ")\n";

	return mapsDecl;
}

std::string VHDLGen::getComponentPortsDeclaration(std::vector<Port*> ports) {
	std::string portsDecl = "";

	portsDecl += "port map (\n";
	for (int i = 0; i < ports.size(); i++) {
		if (ports[i]->getPortSignal() != NULL){
			if (ports[i]->getIsWide() == true)
				portsDecl += "	" + ports[i]->getName() +  "(" + ports[i]->getUsedWidthInitial() + 
							 " downto " + ports[i]->getUsedWidthFinal() + ") => " + 
							 ports[i]->getPortSignal()->getSignalName() + "(" + 
							 ports[i]->getUsedWidthInitial() + " downto " + 
							 ports[i]->getUsedWidthFinal() + ")";
			else if (ports[i]->getUsedWidthInitial() != "-1")
				portsDecl += "	" + ports[i]->getName() +  "(" + ports[i]->getUsedWidthInitial() + 
							 ") => " + ports[i]->getPortSignal()->getSignalName();
			else if (ports[i]->getPortSignal()->getType() == "std_logic_vector" &&
			         ports[i]->getPortSignal()->getDataWidth() == 1 &&
					 ports[i]->getType() != "std_logic_vector")		
				portsDecl += "	" + ports[i]->getName() + " => " + 
							 ports[i]->getPortSignal()->getSignalName() +
							 "(0)";
			else
				portsDecl += "	" + ports[i]->getName() + " => " + 
							 ports[i]->getPortSignal()->getSignalName();
				
							
		    // If its the last decl it doesnt hava a , in the end
			if(i == (ports.size() - 1))
				portsDecl += "\n";
			else
				portsDecl += ",\n";  
		}
	}
	portsDecl += ");\n";

	return portsDecl;
}

void VHDLGen::writeVhdlFile(std::string entityName, std::vector<Component*> components,
	std::vector<Signal*> signals, std::vector<std::string> assignedSignals) {
	std::ofstream file(entityName + ".vhd");

	file << VHDLGen::getLibrariesDeclaration();
	file << VHDLGen::getEntityDeclaration(entityName);

	file << "architecture behavior of " + entityName + " is \n\n";
	
	int ramAux = 0;
	for (int i = 0; i < components.size(); i++) {
		if (components[i]->getComponentName() == "block_ram"){
			// block_ram if first, puts a number in the next ones
			file << components[i]->getVHDLDeclaration(std::to_string(ramAux));
			components[i]->setComponentName("block_ram" + std::to_string(ramAux++));
		}
		else if (components[i]->getIsDeclared() == false)
			file << components[i]->getVHDLDeclaration("");
	}

	file << VHDLGen::getSignalsDeclaration(signals);
	file << "begin\n\n";
	file << VHDLGen::getComponentInstancesDeclaration(components);

	for (int i = 0; i < assignedSignals.size(); i++) {
		file << assignedSignals[i];
	}

	file << "end behavior;";
	file.close();
}

#ifndef VHDLGEN_H
#define	VHDLGEN_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "core/component.hpp"
#include "core/port.hpp"
#include "core/signal.hpp"
#include "core/genericMap.hpp"

class VHDLGen {
public:
	// Only have one instance(singleton)
	static VHDLGen* Instance();

	std::string getLibrariesDeclaration();
	std::string getEntityDeclaration(std::string);
	std::string getSignalsDeclaration(std::vector<Signal*>);
	std::string getComponentInstancesDeclaration(std::vector<Component*>);
	std::string getComponentMapsDeclaration(std::vector<GenericMap*>);
	std::string getComponentPortsDeclaration(std::vector<Port*>);
	void writeVhdlFile(std::string, std::vector<Component*>, std::vector<Signal*>, std::vector<std::string>);

private:
	// Force singleton pattern
	VHDLGen(){};  // Private so that it can  not be called
	VHDLGen(VHDLGen const&){};             // copy constructor is private
	VHDLGen& operator=(VHDLGen const&){};  // assignment operator is private
	static VHDLGen* m_pInstance;
};

#endif	/* VHDLGEN_H */

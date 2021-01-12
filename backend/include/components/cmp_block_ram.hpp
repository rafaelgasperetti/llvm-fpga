#ifndef CMP_BLOCK_RAM_H
#define	CMP_BLOCK_RAM_H

#include <string>
#include "core/component.hpp"

// TODO: Add address width attribute

class Cmp_block_ram : public Component {
public:
	Cmp_block_ram(int dataWidth);
	void setIsDeclared(bool);
	bool getIsDeclared();
	std::string getVHDLDeclaration(std::string);
	void createPorts();
	void createGenericMaps();

private:
	static bool isDeclared;
};

#endif	/* CMP_BLOCK_RAM_H */

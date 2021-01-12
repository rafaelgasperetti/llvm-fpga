#ifndef CMP_MUX_M_H
#define	CMP_MUX_M_H

#include <string>
#include "core/component.hpp"

class Cmp_mux_m : public Component {
public:
	Cmp_mux_m(int dataWidth);
	void setIsDeclared(bool);
	bool getIsDeclared();
	std::string getVHDLDeclaration(std::string);
	void createPorts();
	void createGenericMaps();

private:
	static bool isDeclared;
};

#endif	/* CMP_MUX_M */

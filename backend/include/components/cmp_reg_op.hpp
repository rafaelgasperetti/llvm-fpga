#ifndef CMP_REG_OP_H
#define	CMP_REG_OP_H

#include <string>
#include "core/component.hpp"

class Cmp_reg_op : public Component {
public:
	Cmp_reg_op(int dataWidth);
	void setIsDeclared(bool);
	bool getIsDeclared();
	std::string getVHDLDeclaration(std::string);
	void createPorts();
	void createGenericMaps();

private:
	static bool isDeclared;
};

#endif	/* CMP_REG_OP */

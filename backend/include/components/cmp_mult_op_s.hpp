#ifndef CMP_MULT_OP_S_H
#define	CMP_MULT_OP_S_H

#include <string>
#include "core/component.hpp"

class Cmp_mult_op_s : public Component {
public:
	Cmp_mult_op_s(int dataWidth);
	void setIsDeclared(bool);
	bool getIsDeclared();
	std::string getVHDLDeclaration(std::string);
	void createPorts();
	void createGenericMaps();

private:
	static bool isDeclared;
};

#endif	/* CMP_MULT_OP_S_H */

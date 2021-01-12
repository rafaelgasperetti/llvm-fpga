#ifndef CMP_ADD_REG_OP_S_H
#define	CMP_ADD_REG_OP_S_H

#include<string>
#include<Core/Component.hpp>

using namespace std;

class Cmp_add_reg_op_s : Component {
public:
	Cmp_add_reg_op_s(int dataWidth);
	string GetVHDLDeclaration(string);
	void CreatePorts();
	void CreateGenericMaps();
};

#endif	/* CMP_ADD_REG_OP_S */

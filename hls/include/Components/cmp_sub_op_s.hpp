#ifndef CMP_SUB_OP_S_H
#define	CMP_SUB_OP_S_H

#include<string>
#include<Core/Component.hpp>

using namespace std;

class Cmp_sub_op_s : public Component {
public:
	Cmp_sub_op_s(int dataWidth);
	string GetVHDLDeclaration(string);
	void CreatePorts();
	void CreateGenericMaps();
};

#endif	/* CMP_SUB_OP_S */

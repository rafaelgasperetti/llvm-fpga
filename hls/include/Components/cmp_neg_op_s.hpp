#ifndef CMP_NEG_OP_S_H
#define	CMP_NEG_OP_S_H

#include<string>
#include<Core/Component.hpp>

using namespace std;

class Cmp_neg_op_s : public Component {
public:
	Cmp_neg_op_s(int dataWidth);
	string GetVHDLDeclaration(string);
	void CreatePorts();
	void CreateGenericMaps();
};

#endif	/* CMP_NEG_OP_S */

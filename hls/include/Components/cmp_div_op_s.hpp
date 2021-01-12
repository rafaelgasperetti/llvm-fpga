#ifndef CMP_DIV_OP_S_H
#define	CMP_DIV_OP_S_H

#include<string>
#include<Core/Component.hpp>

using namespace std;

class Cmp_div_op_s : public Component {
public:
	Cmp_div_op_s(int dataWidth);
	string GetVHDLDeclaration(string);
	void CreatePorts();
	void CreateGenericMaps();
};

#endif

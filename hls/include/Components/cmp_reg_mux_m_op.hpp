#ifndef CMP_REG_MUX_M_OP_H
#define	CMP_REG_MUX_M_OP_H

#include<string>
#include<Core/Component.hpp>

using namespace std;

class Cmp_reg_mux_m_op : public Component {
public:
	Cmp_reg_mux_m_op(int dataWidth);
	string GetVHDLDeclaration(string);
	void CreatePorts();
	void CreateGenericMaps();
};

#endif	/* CMP_REG_MUX_M_OP */

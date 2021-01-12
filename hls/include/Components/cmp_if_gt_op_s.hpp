#ifndef CMP_IF_GT_OP_S_H
#define	CMP_IF_GT_OP_S_H

#include<string>
#include<Core/Component.hpp>

using namespace std;

class Cmp_if_gt_op_s : public Component {
public:
	Cmp_if_gt_op_s(int dataWidth);
	string GetVHDLDeclaration(string);
	void CreatePorts();
	void CreateGenericMaps();
};

#endif	/* CMP_IF_GT_OP_S_H */

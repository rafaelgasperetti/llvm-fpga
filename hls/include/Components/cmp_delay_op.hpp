#ifndef CMP_DELAY_OP
#define	CMP_DELAY_OP

#include<string>
#include<Core/Component.hpp>

using namespace std;

class Cmp_delay_op : public Component {
public:
	Cmp_delay_op(int dataWidth);
	string GetVHDLDeclaration(string);
	void CreatePorts();
	void CreateGenericMaps();
};

#endif	/* CMP_DELAY_OP */

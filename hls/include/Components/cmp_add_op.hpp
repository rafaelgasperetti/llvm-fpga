#ifndef CMP_ADD_OP_H
#define	CMP_ADD_OP_H

#include<string>
#include<Core/Component.hpp>

using namespace std;

/**
 * The unsigned ADD operation.
*/
class Cmp_add_op : public Component {
	
	public:
		Cmp_add_op(int);
		string GetVHDLDeclaration();
		void CreatePorts();
		void CreateGenericMaps();
};

#endif

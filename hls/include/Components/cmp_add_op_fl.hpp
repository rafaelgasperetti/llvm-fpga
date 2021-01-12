#include<string>
#include<Core/Component.hpp>

/**
 * The ADD operation for Floating-Point (32-Bits IEEE 754).
 */
class Cmp_add_op_fl : Component {
	public:
		Cmp_add_op_fl(int);
		string GetVHDLDeclaration();
		void CreatePorts();
		void CreateGenericMaps();
};

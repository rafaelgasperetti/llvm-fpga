#ifndef CMP_MUX_M_H
#define	CMP_MUX_M_H

#include<string>
#include<Core/Component.hpp>

using namespace std;

class Cmp_mux_m : public Component {
public:
	Cmp_mux_m(int dataWidth);
	string GetVHDLDeclaration(string);
	void CreatePorts();
	void CreateGenericMaps();
};

#endif	/* CMP_MUX_M */

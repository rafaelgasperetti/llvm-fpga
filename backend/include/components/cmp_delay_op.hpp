#ifndef CMP_DELAY_OP
#define	CMP_DELAY_OP

#include <string>
#include "core/component.hpp"

class Cmp_delay_op : public Component {
public:
	Cmp_delay_op(int dataWidth);
	void setIsDeclared(bool);
	bool getIsDeclared();
	std::string getVHDLDeclaration(std::string);
	void createPorts();
	void createGenericMaps();

private:
	static bool isDeclared;
};

#endif	/* CMP_DELAY_OP */

#ifndef CMP_COUNTER_H
#define	CMP_COUNTER_H

#include <string>
#include "core/component.hpp"

class Cmp_counter : public Component {
public:
	Cmp_counter(int dataWidth);
	void setIsDeclared(bool);
	bool getIsDeclared();
	std::string getVHDLDeclaration(std::string);
	void createPorts();
	void createGenericMaps();

private:
	static bool isDeclared;
};

#endif	/* CMP_COUNTER_H */

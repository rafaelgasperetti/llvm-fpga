#ifndef GENERICMAP_H
#define	GENERICMAP_H

#include <string>

class GenericMap {
public:
	// Constructor
	GenericMap(std::string, std::string, std::string);

	// Setters and getters
	std::string getName();
	void setName(std::string);
	std::string getType();
	void setType(std::string);
	std::string getValue();
	void setValue(std::string);
	
private:
	std::string name;
	std::string type;
	std::string value;
	std::string dataWidth;
};

#endif	/* GENERICMAP_H */
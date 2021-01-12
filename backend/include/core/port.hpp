#ifndef PORT_H
#define	PORT_H

#include <string>
#include <vector>
#include "core/signal.hpp"

class Port {
public:
	// Constructor
	Port(std::string, std::string, std::string, std::string);

	// Setters and getters
	std::string getName();
	void setName(std::string);
	std::string getWidth();
	void setWidth(std::string);
	void setIsWide(bool);
	bool getIsWide();
	std::string getUsedWidthInitial();
	void setUsedWidthInitial(std::string);
	std::string getUsedWidthFinal();
	void setUsedWidthFinal(std::string);
	std::string getType();
	void setType(std::string);
	std::string getPortDirection();
	void setPortDirection(std::string);
	void setPortSignal(Signal*);
	Signal* getPortSignal();
	
private:
	// Attributes of each port
	std::string name;
	std::string width;
	bool isWide;
	std::string usedWidthInitial;
	std::string usedWidthFinal;
	std::string type;
	std::string portDirection;
	Signal* portSignal;
	//std::vector<Signal*> portSignals;
};

#endif	/* PORT_H */

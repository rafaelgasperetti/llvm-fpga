#ifndef CMP_COUNTER_H
#define	CMP_COUNTER_H

#include<string>
#include<Core/IOComponent.hpp>

using namespace std;

class Cmp_counter : public IOComponent {
private:
    float IncrementFactor = 1;
    int Steps = 1;
public:
	Cmp_counter(int dataWidth);
	string GetVHDLDeclaration(string);
	void CreatePorts();
	void CreateGenericMaps();
	static int GetConditionValue(string);
	void SetIntrementFactor(float);
	float GetIncrementFactor();
	void SetSteps(int);
	int GetSteps();
};

#endif	/* CMP_COUNTER_H */

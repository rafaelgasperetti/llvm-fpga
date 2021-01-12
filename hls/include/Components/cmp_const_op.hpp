#include<string>
#include<Core/IOComponent.hpp>
#include<Core/Signal.hpp>
#include<Core/Port.hpp>

using namespace std;

class Cmp_const_op : public IOComponent {

	public:
		Cmp_const_op(string, int);
		Cmp_const_op(int);
		string GetNodeName();
		string GetDotName();
		void SetRelatedSignal(Signal*);
		Signal* GetRelatedSignal();

		string GetVHDLDeclaration(string);
		void CreatePorts();
		void CreateGenericMaps();

    private:
        Signal* RelatedSignal;
};

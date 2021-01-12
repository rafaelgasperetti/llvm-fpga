#include<string>
#include<Core/Component.hpp>
#include<Core/Port.hpp>

#pragma once

using namespace std;

class IOComponent : public Component {

	protected:
		Port* ComponentPort;
		string Type;//inout, in, out
		string Value = "0";

	public:
	    IOComponent();

		string GetValue();
		void SetValue(string);
		string GetFullName();
		Port* GetPort();
		Port* GetDefaultInput();
		Port* GetDefaultOutput();
		void SetType(string);
		string GetType();
		void SetPort(Port*);
};

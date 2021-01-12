#include<string>
#include<Core/Component.hpp>
#include<Core/IOComponent.hpp>

IOComponent::IOComponent() : Component() {
    IoComponent = true;
}

string IOComponent::GetFullName() {
    if (GetPort()->GetType().compare("std_logic"))
    {
        return GetComponentName() + ":" + GetInstanceName();
    }
    else
    {
        return GetComponentName() + ":" + GetInstanceName() + "[" + to_string(GetDataWidth()) + "]";
    }
}

Port* IOComponent::GetPort() {
	return ComponentPort;
}

Port* IOComponent::GetDefaultInput() {
	return ComponentPort;
}

Port* IOComponent::GetDefaultOutput() {
	return ComponentPort;
}

void IOComponent::SetType(string type)
{
    Type = type;
}

string IOComponent::GetType() {
	return Type;
}

void IOComponent::SetPort(Port* port) {
	ComponentPort = port;
	//port->SetComponent(this);
}


string IOComponent::GetValue() {
	return Value;
}

void IOComponent::SetValue(string value) {
	Value = value;
}

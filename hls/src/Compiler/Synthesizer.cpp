#include<iostream>
#include <fstream>
#include<vector>

#include<Tools/MessageLibrary.hpp>
#include<Tools/StringUtils.hpp>
#include<Compiler/Synthesizer.hpp>
#include<FileManager/FileReader.hpp>
#include<FileManager/FileWriter.hpp>
#include<Core/Design.hpp>
#include<Core/Component.hpp>
#include<Core/IOComponent.hpp>
#include<Core/Signal.hpp>
#include<Components/cmp_add_op_s.hpp>
#include<Components/cmp_counter.hpp>
#include<Components/cmp_delay_op.hpp>
#include<Components/cmp_mult_op_s.hpp>
#include<Compiler/NodeVisitor.hpp>

using namespace std;

Synthesizer::Synthesizer(){

}

string Synthesizer::GetLibrariesDeclaration() {
	string libl = "";

	libl += "library IEEE;\n";
	libl += "use IEEE.std_logic_1164.all;\n";
	libl += "use IEEE.std_logic_arith.all;\n";
	libl += "use IEEE.std_logic_unsigned.all;\n";

	return libl;
}

string Synthesizer::GetEntityDeclaration(string entityName, int width) {
	string entity = "";

	entity += "entity " + entityName +" is\n";
	entity += "	port(\n";
	entity += "		\\init\\	: in	std_logic;\n";
	entity += "		\\done\\	: out	std_logic;\n";
	entity += "		\\result\\	: out	std_logic_vector(" + to_string(width - 1) + " downto 0);\n";
	entity += "		\\clk\\		: in	std_logic;\n";
	entity += "		\\reset\\	: in	std_logic;\n";
	entity += "		\\clear\\	: in	std_logic\n";
	entity += "	); \n";
	entity += "end " + entityName + ";\n";

	return entity;
}

string Synthesizer::GetConstantsDeclaration(vector<Component*>* components)
{
    string constsDecl = "";

    for(int i = 0; i < components->size(); i++)
    {
        if((*components)[i]->GetComponentName().compare("const_op") == 0)
        {
            constsDecl += "constant " + (*components)[i]->GetInstanceName() + " : std_logic_vector(" + to_string((*components)[i]->GetDataWidth() - 1) + " downto 0);\n";
        }
    }

    constsDecl += "\n";

    return constsDecl;
}

string Synthesizer::GetSignalsDeclaration(vector<Signal*>* signals) {
	string signalsDecl = "";

	for (int i = 0; i < signals->size(); i++) {
		if((*signals)[i]->GetSignalName() == "\\init\\" ||
			(*signals)[i]->GetSignalName() == "\\done\\" ||
			(*signals)[i]->GetSignalName() == "\\result\\" ||
			(*signals)[i]->GetSignalName() == "\\clk\\" ||
			(*signals)[i]->GetSignalName() == "\\reset\\" ||
			(*signals)[i]->GetSignalName() == "\\clear\\" )
			continue;

		signalsDecl += "signal " + (*signals)[i]->GetSignalName() + "	: " + (*signals)[i]->GetType();
		if ((*signals)[i]->GetType() == "std_logic_vector") {
			if ((*signals)[i]->GetDataWidth() == 1)
				signalsDecl += "(0 downto 0);\n";
			else
				signalsDecl += "(" + to_string((*signals)[i]->GetDataWidth() - 1) +
				" downto 0);\n";
		}
		else {
			signalsDecl += ";\n";
		}
	}

	signalsDecl += "\n";

	return signalsDecl;
}

string Synthesizer::GetComponentInstancesDeclaration(vector<Component*>* components) {
	string componentDecl = "";

	for (int i = 0, ramNumber = 0; i < components->size(); i++) {
        if((*components)[i]->GetVHDLDeclaration((*components)[i]->GetComponentName()).compare("") != 0)
        {
            string instanceType = ((*components)[i]->GetComponentName().compare("block_ram") == 0 ? (*components)[i]->GetComponentName() + "_" + to_string(ramNumber) : (*components)[i]->GetComponentName());
            componentDecl += "\t" + (*components)[i]->GetInstanceName() + ": " + instanceType + "\n";

            if((*components)[i]->GetComponentName().compare("block_ram") == 0)
            {
                ramNumber++;
            }

            componentDecl += GetComponentMapsDeclaration((*components)[i]->GetMaps());
            componentDecl += GetComponentPortsDeclaration((*components)[i]->GetPorts());
            componentDecl += "\n";
        }
	}
	return componentDecl;
}

string Synthesizer::GetComponentMapsDeclaration(vector<GenericMap*> maps) {
	string mapsDecl = "";

    if(maps.size() > 0)
    {
        mapsDecl += "\tgeneric map (\n";
        for (int i = 0; i < maps.size(); i++) {
            mapsDecl += "\t\t" + maps[i]->GetName() + " => " + maps[i]->GetValue();

            // If its the last decl it doesnt hava a , in the end
            if(i == (maps.size() - 1))
                mapsDecl += "\n";
            else
                mapsDecl += ",\n";
        }
        mapsDecl += "\t)\n";
    }

	return mapsDecl;
}

string Synthesizer::GetComponentPortsDeclaration(vector<Port*> ports) {
	string portsDecl = "";

    if(ports.size() > 0)
    {
        portsDecl += "\tport map (\n";
        for (int i = 0; i < ports.size(); i++)
        {
            for(Signal* s : ports[i]->GetPortSignals())
            {
                if(s != NULL)
                {
                    if(i > 0)
                    {
                        portsDecl = portsDecl + ",\n";
                    }

                    if(ports[i]->GetWidth() == s->GetDataWidth())
                    {
                        portsDecl += "\t\t" + ports[i]->GetName() + " => " + s->GetSignalName();
                    }
                    else if(ports[i]->GetWidth() > 1 && s->GetDataWidth() == 1)
                    {
                        portsDecl += "\t\t" + ports[i]->GetName() +  "(" + to_string(s->GetDataWidth() - 1) + " downto " + ports[i]->GetUsedWidthFinal() + ") => " +
                                     s->GetSignalName();
                    }
                    else if(ports[i]->GetWidth() == 1 && s->GetDataWidth() > 1)
                    {
                        portsDecl += "\t\t" + ports[i]->GetName() + " => " +
                                     s->GetSignalName() + "(" + to_string(ports[i]->GetWidth() - 1) + ")";
                    }
                    else if(ports[i]->GetWidth() > s->GetDataWidth())
                    {
                        portsDecl += "\t\t" + ports[i]->GetName() +  "(" + to_string(s->GetDataWidth() - 1) + " downto 0) => " +
                                     s->GetSignalName() + "(" + to_string(s->GetDataWidth() - 1) + " downto 0)";
                    }
                    else
                    {
                        portsDecl += "\t\t" + ports[i]->GetName() +  "(" + ports[i]->GetUsedWidthInitial() + " downto " + ports[i]->GetUsedWidthFinal() + ") => " +
                                     s->GetSignalName() + "(" + ports[i]->GetUsedWidthInitial() + " downto " + ports[i]->GetUsedWidthFinal() + ")";
                    }
                }
            }

            if(i == ports.size() - 1)
            {
                portsDecl += "\n";
            }
        }
        portsDecl += "\t);\n";
    }

	return portsDecl;
}

string Synthesizer::GetExternalComponentsDeclaration(vector<Component*>* components)
{
    string output = "";
    vector<string> declaredTypes;

    for (int i = 0, ramNumber = 0; i < components->size(); i++)
    {
        if(StringUtils::IndexOf((*components)[i]->GetComponentName(), declaredTypes) == -1)
        {
            if ((*components)[i]->GetComponentName() == "block_ram")
            {
                output += (*components)[i]->GetVHDLDeclaration(to_string(ramNumber));
                declaredTypes.push_back((*components)[i]->GetComponentName() + "_" + to_string(ramNumber));
                ramNumber++;
            }
            else if ((*components)[i]->IsDeclared() == false)
            {
                output += (*components)[i]->GetVHDLDeclaration("");
                declaredTypes.push_back((*components)[i]->GetComponentName());
            }
        }
	}

	return output;
}

string Synthesizer::GetDesignDeclaration(Design* design)
{
    string output = "";
    vector<Component*>* components = GetComponents(design);

    output += GetLibrariesDeclaration() + "\n";
    output += GetEntityDeclaration(design->GetName(), design->GetWidth()) + "\n";
    output += "architecture behavior of " + design->GetName() + " is \n";
    output += GetExternalComponentsDeclaration(components);
    output += GetConstantsDeclaration(components);
    output += GetSignalsDeclaration(design->GetSignals());
    output += "begin\n";
    output += GetComponentInstancesDeclaration(components);
    output += "end behavior;";

    return output;
}

void Synthesizer::CreateMemoryFile(vector<Cmp_block_ram*> memories, FileWriter* mainWriter)
{
    if(memories.size() > 0)
    {
        string memoryPath = mainWriter->GetParentPaths() + mainWriter->GetPathSeparator() + "memory.vhd";
        FileWriter* memoryWriter = new FileWriter(memoryPath);

        if(memoryWriter->Exists())
        {
            cout << "Deleting old generated VHDL memory code file from \"" << memoryWriter->GetPath() << "\"..." << endl;
            memoryWriter->Delete();
            cout << "Deleted old generated VHDL memory code file from \"" << memoryWriter->GetPath() << "\"." << endl << endl;
        }

        cout << "Generating design's VHDL memory code and writing into output file \"" << memoryWriter->GetPath() << "\"..." << endl;

        string memoryCode = "";

        memoryCode += GetLibrariesDeclaration() + "\n";

        for(int i = 0; i < memories.size(); i++)
        {
            if(i == 0)
            {
                memoryCode += Cmp_block_ram::GetEntityDeclaration(memories[i], i);
            }
            else
            {
                memoryCode += "\n\n" + Cmp_block_ram::GetEntityDeclaration(memories[i], i);
            }
        }

        memoryWriter->WriteFile(memoryCode);

        cout << "Generated design's VHDL memory code and written into output file \"" << memoryWriter->GetPath() << "\"." << endl << endl;
    }
}

vector<Component*>* Synthesizer::GetComponents(Design* design)
{
    vector<Component*>* components = new vector<Component*>();
    for(ComponentIdentifier c : (*design->GetComponents()))
    {
        components->push_back(c.IdentifiedComponent);
    }

    return components;
}

//Executes the HLS using the CLang lib.
//Example of how to use the library: https://gist.github.com/raphaelmor/3150866
int Synthesizer::Synthesize(int argc, char** argv)
{
    vector<string> args = StringUtils::GetArgs(1, argv);

    if(argc == 2 && args.size() >= 2)
    {
        int designWidth = 32;
        if(args.size() >= 3 && stoi(args[2]) > 0)
        {
            designWidth = stoi(args[2]);
        }

        // Hardware design of the program
        Design* design = NodeVisitor::VisitAll(StringUtils::StringToChar(args[0]), designWidth);

        // Putting only the components on the vector
        vector<Component*>* componentArray = GetComponents(design);

        cout << endl << "---------Design (" << to_string(designWidth) << " bits) check---------" << endl;

        for(Component* c : (*componentArray))
        {
            if(c->IsBlockRam())
            {
                string values;
                for(int i = 0; i < c->AssignedConstants.size(); i++)
                {
                    values += "\t" + c->AssignedConstants[i];
                }
                if(c->AssignedConstants.size() > 0)
                {
                    cout << "Declared ram component name: " << c->GetInstanceName() << "(" << c->GetComponentName() << " with values " << endl << values << ", maxSize: " << ((Cmp_block_ram*) c)->GetMaxSize() << ")" << endl;
                }
                else
                {
                    cout << "Declared ram component name: " << c->GetInstanceName()  << " (" << c->GetComponentName() << ", maxSize: " << ((Cmp_block_ram*) c)->GetMaxSize() << ")" << endl;
                }
            }
            else if(c->IsIoComponent())
            {
                cout << "Declared component name: " << c->GetInstanceName()  << " (" << c->GetComponentName() << " with value \"" << ((IOComponent*) c)->GetValue() << "\")" << endl;
            }
            else
            {
                cout << "Declared component name: " << c->GetInstanceName()  << " (" << c->GetComponentName() << ")" << endl;
            }

            if(c->GetComponentName().compare("const_op") != 0)
            {
                for(Port* p : c->GetPorts())
                {
                    cout << (p->GetPortSignals().size() == 0 ? "\tPort: " + p->GetName() + ", Signal: No signal connected\n" : "");
                    for(Signal* s : p->GetPortSignals())
                    {
                        cout << "\tPort: " << p->GetName() << ", Signal: " << s->GetSignalName() << endl;
                    }
                }
            }
        }

        cout << "-----End of design (" << to_string(designWidth) << " bits) check------" << endl << endl;

        FileWriter* writer = new FileWriter(args[1]);
        design->SetName(writer->GetFileName(false));

        if(writer->Exists())
        {
            cout << "Deleting old generated VHDL code file from \"" << writer->GetPath() << "\"..." << endl;
            writer->Delete();
            cout << "Deleted old generated VHDL code file from \"" << writer->GetPath() << "\"." << endl << endl;
        }

        vector<Cmp_block_ram*> memories;

        for(Component* c : (*componentArray))
        {
            if(c->IsBlockRam())
            {
                memories.push_back((Cmp_block_ram*) c);
            }
        }

        CreateMemoryFile(memories, writer);

        cout << "Generating design's VHDL code and writing into output file \"" << writer->GetPath() << "\"..." << endl;
        writer->WriteFile(GetDesignDeclaration(design));
        cout << "Generated design's VHDL code and written into output file \"" << writer->GetPath() << "\"." << endl << endl;

        //Returns successful HLS result.
        cout << "Synthesis successfully executed." << endl << endl;
        return MessageLibrary::COMPILATION_SUCCESSFULL;
    }
    else
    {
        //Returns error HLS result.
        cerr << "Synthesis error \"ERROR_NOT_SPECIFIED_SOURCE\" (" << MessageLibrary::ERROR_NOT_SPECIFIED_SOURCE << ")." << endl;
        return MessageLibrary::ERROR_NOT_SPECIFIED_SOURCE;
    }
}

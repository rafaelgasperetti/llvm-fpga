#ifndef SYNTHESIZER_H
#define	SYNTHESIZER_H

#include<string>
#include<vector>

#include<Core/Design.hpp>
#include<Core/Signal.hpp>
#include<Core/Component.hpp>
#include<Core/Port.hpp>

#include<Components/cmp_block_ram.hpp>

#include<FileManager/FileWriter.hpp>

using namespace std;

class Synthesizer
{
	public:
		inline Synthesizer();
		inline int Synthesize(int, char**);

    private:
        //Variables

        //Methods
        inline string GetLibrariesDeclaration();
        inline string GetEntityDeclaration(string, int);
        inline string GetConstantsDeclaration(vector<Component*>*);
        inline string GetSignalsDeclaration(vector<Signal*>*);
        inline string GetComponentInstancesDeclaration(vector<Component*>*);
        inline string GetComponentMapsDeclaration(vector<GenericMap*>);
        inline string GetComponentPortsDeclaration(vector<Port*>);
        string GetExternalComponentsDeclaration(vector<Component*>*);
        inline string GetDesignDeclaration(Design*);
        void CreateMemoryFile(vector<Cmp_block_ram*>, FileWriter*);
        inline vector<Component*>* GetComponents(Design*);
};

#endif

#ifndef CMP_BLOCK_RAM_H
#define	CMP_BLOCK_RAM_H

#include<string>
#include<Core/Component.hpp>

#include<Tools/StringUtils.hpp>

// TODO: Add address width attribute

using namespace std;

class Cmp_block_ram : public Component {
    public:
        Cmp_block_ram(int dataWidth);
        string GetVHDLDeclaration(string);
        void CreatePorts();
        void CreateGenericMaps();
        void SetMaxSize(int);
        int GetMaxSize();

        static string GetEntityDeclaration(Cmp_block_ram*, int);

    private:
        int MaxSize;
};

#endif	/* CMP_BLOCK_RAM_H */

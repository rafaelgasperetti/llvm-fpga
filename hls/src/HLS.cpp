#include<stdio.h>
#include<stdlib.h>
#include<stdexcept>
#include<iostream>

#include<Tools/MessageLibrary.hpp>
#include<Compiler/Synthesizer.cpp>

using namespace std;

int main(int argc, char** argv)
{
    try
	{
	    //Instancia��o do sintetizador de alto n�vel.
		Synthesizer synthesizer;
		return synthesizer.Synthesize(argc, argv);//Futuramente isso ser� substitu�do por um par�metro de entrada.
	}
	catch(invalid_argument& e)
	{
		cerr << "Error at file reading: " << e.what() << endl;
		return MessageLibrary::ERROR_FILE_READING;
	}
}

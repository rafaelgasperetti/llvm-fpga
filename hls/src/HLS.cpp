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
	    //Instanciação do sintetizador de alto nível.
		Synthesizer synthesizer;
		return synthesizer.Synthesize(argc, argv);//Futuramente isso será substituído por um parâmetro de entrada.
	}
	catch(invalid_argument& e)
	{
		cerr << "Error at file reading: " << e.what() << endl;
		return MessageLibrary::ERROR_FILE_READING;
	}
}

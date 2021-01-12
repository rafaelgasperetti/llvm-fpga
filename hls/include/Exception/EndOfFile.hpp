#include<iostream>
#include<exception>

using namespace std;

class EndOfFile : public exception
{
    public:
        string Message;
        EndOfFile(string message)
        {
            Message = message;
        }

        virtual const char* what()
        const throw()
        {
            return Message.c_str();
        }
};

#include<string>
#include<vector>

using namespace std;

class TokenCursor
{
    public:
        TokenCursor(string);
        bool Read();
        string GetValue();

    private:
        const vector<string> TokenSeparator =
        {
            " ",".","::",":","{","}","(",")","[","]","\"","'","\n","\t","&&","&","||","|","+","-","*","/","!","@",",","#","%"
        };
        string Text;
        string Value;
        bool CanRead;
        int CursorIndex = 0;
        int TokenSeparatorLength(string);
};

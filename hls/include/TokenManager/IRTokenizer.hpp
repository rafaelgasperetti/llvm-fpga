#include<string>
#include<vector>

#include<FileManager/FileReader.hpp>
#include<TokenManager/TokenCursor.hpp>

using namespace std;

class IRTokenizer
{
    public:
        IRTokenizer();
        vector<string> GetTokens(FileReader*);

    private:
        vector<string> Tokens;
        const vector<string> IgnoredTokens =
        {
            " ","\n","\t"
        };
};

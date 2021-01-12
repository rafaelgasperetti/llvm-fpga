#include<TokenManager/IRTokenizer.hpp>
#include<Tools/StringUtils.hpp>

using namespace std;

IRTokenizer::IRTokenizer()
{

}

vector<string> IRTokenizer::GetTokens(FileReader* reader)
{
    if(reader != NULL)
    {
        string text = reader->ReadFile();//(FileReader::READ_CONFIG_IGNORE_STARTING_WITH, ";", true);
        //string text = reader->ReadFile();
        if(text.empty())
        {
            throw invalid_argument("It was not possible to get the tokens from the File \"" + reader->GetPath() + "\" because the file is empty.");
        }
        else
        {
            TokenCursor* cursor = new TokenCursor(text);

            while(cursor->Read())
            {
                if(!StringUtils::Contains(cursor->GetValue(),IgnoredTokens))
                {
                    Tokens.push_back(cursor->GetValue());
                }
            }

            return Tokens;
        }
    }
    else
    {
        throw invalid_argument("It was not possible to get the tokens from FileReader because it is null.");
    }
}

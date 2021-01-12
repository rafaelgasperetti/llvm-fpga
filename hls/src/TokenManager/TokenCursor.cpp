#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<stdexcept>

#include<TokenManager/TokenCursor.hpp>
#include<Tools/StringUtils.hpp>

using namespace std;

TokenCursor::TokenCursor(string text)
{
    if(!text.empty())
    {
        Text = text;
        CanRead = false;
    }
    else
    {
        throw invalid_argument("The file reader must never be null on a TokenCursor.");
    }
}

int TokenCursor::TokenSeparatorLength(string token)
{
    for(int i = 0; i < TokenSeparator.size(); i++)
    {
        if(StringUtils::EndsWith(token,TokenSeparator[i]))
        {
            return TokenSeparator[i].length();
        }
    }
    return -1;
}

bool TokenCursor::Read()
{
    int moves = 0;
    int separatorLen = TokenSeparatorLength(Text.substr(CursorIndex, moves));
    while(separatorLen == -1 && CursorIndex + moves < Text.length())
    {
        moves++;
        separatorLen = TokenSeparatorLength(Text.substr(CursorIndex, moves));
    }
    if(separatorLen > 0)
    {
        bool valueIsSeparator = StringUtils::Contains(Text.substr(CursorIndex, moves),TokenSeparator);
        Value =  valueIsSeparator ? Text.substr(CursorIndex, moves) : Text.substr(CursorIndex, (moves - separatorLen >= 0 ? moves - separatorLen : 0));
        CursorIndex = valueIsSeparator ? CursorIndex + moves : CursorIndex + moves - separatorLen;
        CanRead = true;
        return true;
    }
    else
    {
        CanRead = false;
        return false;
    }
}

string TokenCursor::GetValue()
{
    if(CanRead)
    {
        return Value;
    }
    else
    {
//        throw invalid_argument("The cursor was not readable at the moment.");
    }
}

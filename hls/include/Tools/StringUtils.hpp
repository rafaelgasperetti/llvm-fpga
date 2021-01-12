#include<string.h>
#include<vector>

using namespace std;

#pragma once

class StringUtils
{
    public:
        static char* StringToChar(string st)
        {
            char* ret = new char[st.size() + 1];
            strcpy(ret, st.c_str());
            return ret;
        }

        static string CharToString(char* st)
        {
            string ret(st);
            return ret;
        }

        static int StringToInt(string st)
        {
            return atoi(StringToChar(st));
        }

        static string IntToString(int i)
        {
            return to_string(i);
        }

        static bool Equals(string str, string cmp)
        {
            if
            (
                str.compare(cmp) == 0
            )
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        static bool StartsWith(string str, string cmp)
        {
            if
            (
                str.length() >= cmp.length() &&
                str.substr(0,cmp.length()).compare(cmp) == 0
            )
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        static bool EndsWith(string str, string cmp)
        {
            if
            (
                str.length() >= cmp.length() &&
                str.substr(str.length() - cmp.length(), cmp.length()).compare(cmp) == 0
            )
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        static string AppendLine(string st1,string st2,bool newLineBetween, bool newLineAfter, bool whenEmptyIgnoreNewLine)
        {
            string ret = "";
            if(newLineBetween)// && (!whenEmptyIgnoreNewLine || (whenEmptyIgnoreNewLine && (st1.empty() || st2.empty()))))
            {
                ret = st1 + '\n' + st2;
            }
            else
            {
                ret = st1 + st2;
            }

            if(newLineAfter && (!whenEmptyIgnoreNewLine || (whenEmptyIgnoreNewLine && st2.empty())))
            {
                ret = ret + '\n';
            }
            return ret;
        }

        static string AppendLine(string st1,string st2)
        {
            return AppendLine(st1, st2, false, false, false);
        }

        static bool Contains(string st, vector<string> v)
        {
            for(string &s : v)
            {
                if(Equals(st,s))
                {
                    return true;
                }
            }
            return false;
        }

        static bool Contains(string st1, string st2)
        {
            size_t found = st1.find(st2);
            if (found!=string::npos)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        static int IndexOf(string st, vector<string> v)
        {
            for(unsigned int i = 0; i < v.size(); i++)
            {
                if(Equals(st,v[i]))
                {
                    return i;
                }
            }
            return -1;
        }

        static int IndexOf(string st1, string st2)
        {
            if(st2.size() <= st1.size())
            {
                for(unsigned int i = 0; i + st2.size() <= st1.size(); i++)
                {
                    if(Equals(st1.substr(i, st2.size()), st2))
                    {
                        return i;
                    }
                }
            }
            return -1;
        }

        static vector<int> IndexesOf(string st1, string st2)
        {
            vector<int> indexes;
            if(st2.size() <= st1.size())
            {
                for(unsigned int i = 0; i + st2.size() <= st1.size(); i++)
                {
                    if(Equals(st1.substr(i, st2.size()), st2))
                    {
                        indexes.push_back(i);
                    }
                }
            }
            return indexes;
        }

        static int LastIndexOf(string st, vector<string> v)
        {
            int ret = -1;
            for(unsigned int i = 0; i < v.size(); i++)
            {
                if(Equals(st,v[i]))
                {
                    ret = i;
                }
            }
            return ret;
        }

        static int LastIndexOf(string st1, string st2)
        {
            int idx = -1;
            if(st2.size() <= st1.size())
            {
                for(unsigned int i = 0; i + st2.size() <= st1.size(); i++)
                {
                    if(Equals(st1.substr(i, st2.size()), st2))
                    {
                        idx = i;
                    }
                }
            }
            return idx;
        }

        static string Replace(string base, string from, string to)
        {
            string SecureCopy = base;

            for (size_t start_pos = SecureCopy.find(from); start_pos != string::npos; start_pos = SecureCopy.find(from,start_pos))
            {
                SecureCopy.replace(start_pos, from.length(), to);
            }

            return SecureCopy;
        }

        static vector<string> GetArgs(int argNumber, char** argv)
        {
            vector<string> ret;

            if(argv != NULL)
            {
                string argv1(argv[argNumber]);
                vector<int> indexes = StringUtils::IndexesOf(argv1,"\"");

                if(indexes.size() > 0 && indexes[0] == 0)
                {
                    for(int i = 1; i <= indexes.size(); i++)
                    {
                        if(i % 2 == 1)
                        {
                            string newArg = argv1.substr(indexes[i - 1] + 1, indexes[i] - indexes[i - 1] - 1);
                            ret.push_back(newArg);
                        }
                    }
                }
                else
                {
                    vector<int> spaceIdxs = StringUtils::IndexesOf(argv1," ");

                    for(int i = 0; i <= spaceIdxs.size(); i++)
                    {
                        string newArg;
                        if(i == 0)
                        {
                            newArg = argv1.substr(0, spaceIdxs[i]);
                        }
                        else
                        {
                            newArg = argv1.substr(spaceIdxs[i - 1] + 1, spaceIdxs[i] - spaceIdxs[i - 1] - 1);
                        }
                        ret.push_back(newArg);
                    }
                }
            }

            return ret;
        }

        static string Replicate(string base, int times)
        {
            string ret = "";
            if(times < 0)
            {
                times = 0;
            }

            for(int i = 0; i < times; i++)
            {
                ret += base;
            }

            return ret;
        }
};

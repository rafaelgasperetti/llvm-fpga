#include<FileManager/FileReader.hpp>
#include<Exception/EndOfFile.hpp>
#include<Tools/StringUtils.hpp>

using namespace std;

FileReader::FileReader(string path) : FileBaseManager::FileBaseManager(path)
{

}

string FileReader::ReadFileLine(ifstream& file, int lineNumber)
{
    string line = "";

    if (file.is_open())
	{
	    if(getline(file,line))
        {
            return line;
        }
        else
        {
            throw EndOfFile("The file was successfully ridden.");
        }
	}
	else
	{
		if(lineNumber >= 0)
        {
            stringstream ss;
            ss << lineNumber;
            throw invalid_argument("The file \"" + GetPath() + "\" is not opened at line " + ss.str() + ".");
        }
        else
        {
            throw invalid_argument("The file \"" + GetPath() + "\" is not opened.");
        }
	}
}

string FileReader::ReadFileLine(ifstream& file)
{
    return ReadFileLine(file,-1);
}

string FileReader::ReadFile(const int readConfig, string pattern, bool ignoreEmptyLines)
{
    int lineNumber = 0;
	string ret = "",line;

	ifstream file;
	file.open(GetPath().c_str());

    try
    {
        while(true)
        {
            line = ReadFileLine(file);

            if(ignoreEmptyLines || !line.empty())
            {
                switch(readConfig)
                {
                    case READ_CONFIG_IGNORE_STARTING_WITH:
                        if(!StringUtils::StartsWith(line,pattern))
                        {
                            ret = StringUtils::AppendLine(ret, line, lineNumber != 0, false, true);
                            lineNumber++;
                        }
                        break;
                    default:
                        ret = StringUtils::AppendLine(ret, line, lineNumber != 0, false, true);
                        lineNumber++;
                        break;
                }
            }

            line = "";

        }
    }
    catch(EndOfFile& e)
    {
        //Nada... é previsto acontecer quando o arquivo finalizar a leitura.
    }

	if (file.is_open())
	{
        file.close();
	}

	return ret;
}

string FileReader::ReadFile()
{
	return ReadFile(READ_CONFIG_DO_NOTHING, "", false);
}

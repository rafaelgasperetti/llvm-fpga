#include<FileManager/FileBaseManager.hpp>

#include<Tools/StringUtils.hpp>

using namespace std;

const char* FileBaseManager::OPEN_TYPE_READ = "r";
const char* FileBaseManager::OPEN_TYPE_WRITE ="w";

FileBaseManager::FileBaseManager(string path)
{
    SetPath(path);
}

string FileBaseManager::GetPath()
{
    return Path;
}

string FileBaseManager::GetPathSeparator()
{
    if(StringUtils::Contains(GetPath(), "/"))
    {
        return "/";
    }
    else
    {
        return "\\";
    }
}

string FileBaseManager::GetFileName(bool withExtension)
{
    int lastIdxSeparator = StringUtils::LastIndexOf(GetPath(), GetPathSeparator());
    int lastIdxDot = -1;

    if(withExtension)
    {
        return GetPath().substr(lastIdxSeparator + 1, GetPath().size() - lastIdxSeparator + 1);
    }
    else
    {

        lastIdxDot = StringUtils::LastIndexOf(GetPath(), ".");
        int substringSize = lastIdxDot > 0 ? lastIdxDot - lastIdxSeparator : GetPath().size() - lastIdxSeparator;
        return GetPath().substr(lastIdxSeparator + 1, substringSize - 1);

    }
}

string FileBaseManager::GetParentPaths()
{
    int lastIdxSeparator = StringUtils::LastIndexOf(GetPath(), GetPathSeparator());
    return GetPath().substr(0, lastIdxSeparator);
}

string FileBaseManager::GetExtension()
{
    if(StringUtils::Contains(GetPath(),"."))
    {
        int lastIdxDot = StringUtils::LastIndexOf(GetPath(), ".");
        return GetPath().substr(lastIdxDot, GetPath().size() - lastIdxDot);
    }
    else
    {
        return "";
    }
}

void FileBaseManager::SetPath(string path)
{
    if(!path.empty())
    {
        Path = path;
    }
    else
    {
        throw new std::invalid_argument("The FileWriter path must never be null.");
    }
}

bool FileBaseManager::Exists()
{
    FILE *file;
    if ((file = fopen(StringUtils::StringToChar(GetPath()), FileBaseManager::OPEN_TYPE_READ)))
    {
        fclose(file);
        return true;
    }
    return false;
}

bool FileBaseManager::Delete()
{
    if(remove(StringUtils::StringToChar(GetPath())) != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

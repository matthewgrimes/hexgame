#include "FileLoader.h"
#include "HexUtils.h"
FileLoader::FileLoader(const std::string& name)
{
    fileName = name;
}

FileLoader::~FileLoader()
{
    //dtor
}

bool FileLoader::openFile()
{
    if ( fileName.empty() )
    {
        return false;
    }

    m_file.open(fileName.c_str());

    return true;
}

void FileLoader::closeFile()
{
    m_file.close();
}

void FileLoader::parseFile()
{
    std::vector<std::string> content;
    std::string line, field, data;

    openFile();
    while ( std::getline(m_file, line) )
    {
        // only parse if the line isn't a comment
        if ( line.compare(0,1,"#")!=0 )
        {
            separateLineHeader(line, &field, &data);
            parseLine(field, data);
        }

    }
    closeFile();
}

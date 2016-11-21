#ifndef FILELOADER_H
#define FILELOADER_H


#include "LineParser.h"
#include <string>
#include <fstream>
#include <vector>

class FileLoader
{
public:
    FileLoader(const std::string& name);

    virtual ~FileLoader();

    void parseFile();
protected:
    std::string fileName;
    std::ifstream m_file;

    bool openFile();
    void closeFile();

    virtual void parseLine(const std::string& field, const std::string& data)=0;
private:
};

#endif // FILELOADER_H

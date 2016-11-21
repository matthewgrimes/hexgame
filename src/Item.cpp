#include "Item.h"

void ItemFileLoader::parseLine(const std::string& field, const std::string& data)
{
    if ( field == "name" )
    {
        item->name = ParseString(data);
    }
}

Item::Item()
{

}

Item::~Item()
{

}

void Item::setName(std::string value)
{
    name = value;
}

std::string Item::getName()
{
    return name;
}

bool Item::loadFromFile(const std::string& fileName)
{
    ItemFileLoader loader(fileName);
    loader.setItem(this);
    loader.parseFile();

    return 1;
}

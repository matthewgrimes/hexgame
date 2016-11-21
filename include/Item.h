#ifndef ITEM_H
#define ITEM_H

#include "FileLoader.h"
#include <string>

//! a generic item class
class Item
{
    friend class ItemFileLoader;
public:
    Item();
    virtual ~Item();

    virtual bool loadFromFile(const std::string& fileName);

    // return name
    std::string getName();
    // set name
    void setName(std::string value);
protected:
    std::string name;
};

class ItemFileLoader : public virtual FileLoader
{
public:
    ItemFileLoader(const std::string& name) : FileLoader(name) {}
    ~ItemFileLoader() {}

    void setItem(Item* value)
    {
        item = value;
    }
protected:
    Item* item;
    virtual void parseLine(const std::string& field, const std::string& data);
};
#endif

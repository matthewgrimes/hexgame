#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include "damageEffect.h"
#include "FileLoader.h"
#include <string>
#include <vector>

class Technique
{
    friend class TechniqueFileLoader;
public:
    /** Default constructor */
    Technique();
    /** Default destructor */
    virtual ~Technique();

    void loadFromFile(const std::string& fileName);

    std::string getName()
    {
        return Name;
    }
    int getRange()
    {
        return range;
    }
    int getDamage()
    {
        return effect.getBaseHealthEffectAmount();
    }
    int getAOE()
    {
        return effect.getAOE();
    }

    damageEffect getDamageEffect()
    {
        return effect;
    }
protected:
    std::string Name;
    damageEffect effect;
    int range;
private:
};

class TechniqueFileLoader : public FileLoader
{
public:
    TechniqueFileLoader(const std::string& name) : FileLoader(name) {}
    ~TechniqueFileLoader() {}

    void setTechnique(Technique* value)
    {
        technique = value;
    }
protected:
    Technique* technique;
    void parseLine(const std::string& field, const std::string& data);
};

#endif // TECHNIQUE_H

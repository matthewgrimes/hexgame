#ifndef WEAPON_H
#define WEAPON_H

#include "Item.h"
#include "damageEffect.h"
#include "FileLoader.h"

class Character;
enum weaponTypes { MELEE, RANGED };

class Weapon : public Item
{
    friend class WeaponFileLoader;
public:
    Weapon() {};
    virtual ~Weapon() {};
    virtual bool loadFromFile(const std::string& fileName);

    int getMinRange();
    int getMaxRange();
    int getDamage();
    int getAOE();
    weaponTypes getType();

    damageEffect getDamageEffect()
    {
        return damage;
    }
protected:
    int minRange;
    int maxRange;

    damageEffect damage;
    weaponTypes type;
};

class WeaponFileLoader : public ItemFileLoader
{
public:
    WeaponFileLoader(const std::string& name) : FileLoader(name), ItemFileLoader(name) {}
    ~WeaponFileLoader() {}

    void setWeapon(Weapon* value)
    {
        weapon = value;
        item = value;
    }
protected:
    void parseLine(const std::string& field, const std::string& data);
    Weapon* weapon;
};
#endif

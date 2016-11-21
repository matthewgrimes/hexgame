#ifndef ATTACK_H
#define ATTACK_H

#include "damageEffect.h"
#include <string>

class Character;

class Attack
{
public:
    Attack() {}
    ~Attack() {}

    void perform(Character* source, Character* target);
    int calculateDamage(Character* source, Character* target);

    int getMinRange()
    {
        return minRange;
    }
    int getMaxRange()
    {
        return maxRange;
    }
    int getDamage()
    {
        return damage.getBaseHealthEffectAmount();
    }
    int getAOE()
    {
        return damage.getAOE();
    }
    std::string getName()
    {
        return name;
    }
    bool checkRange(int i1, int j1, int i2, int j2);

    void setMinRange(int value)
    {
        minRange = value;
    }
    void setMaxRange(int value)
    {
        maxRange = value;
    }
    void setDamage(damageEffect value)
    {
        damage = value;
    }
    void setName(std::string value)
    {
        name = value;
    }
protected:
    int minRange;
    int maxRange;

    damageEffect damage;

    std::string name;

};

#endif

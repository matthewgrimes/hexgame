#ifndef CHARACTER_H
#define CHARACTER_H

#include "Actor.h"
#include "Item.h"
#include "Weapon.h"
#include "Technique.h"
#include "FileLoader.h"
#include "Attack.h"
#include <string>
#include <utility>
#include <map>

struct Stats
{
    Stats() : hitPoints(0), maxHitPoints(0), speed(0), agility(0), damageReduction(0), damageMitigation(0.f) {}

    int hitPoints, maxHitPoints;
    int speed;
    int agility;

    int damageReduction;
    int damageMitigation;
};

class Character : public Actor //!< Organizes the concept of a character: stats, inventory, etc
{
    friend class CharacterFileLoader;
public:
    /** Constructor */
    Character();
    /** Destructor */
    ~Character();

    void Render();


    bool loadFromFile(const std::string& fileName);

    int getSpeed()
    {
        return stats.speed;
    }
    void setSpeed(int value)
    {
        stats.speed = value;
    }

    int getAgility()
    {
        return stats.agility;
    }
    void setAgility(int value)
    {
        stats.agility = value;
    }

    int getDamageMitigation()
    {
        return stats.damageMitigation;
    }
    int getDamageReduction()
    {
        return stats.damageReduction;
    }

    std::string getName()
    {
        return name;
    }
    void setName(const std::string& value)
    {
        name = value;
    }

    void setHitPoints(int value)
    {
        stats.hitPoints = value;
    }
    void decreaseHitPoints(int value);
    void increaseHitPoints(int value);
    int getHitPoints()
    {
        return stats.hitPoints;
    }
    const float getHitPointsPercent() const
    {
        return (float)(stats.hitPoints)/(float)(stats.maxHitPoints);
    }

    int getMaxHitPoints()
    {
        return stats.maxHitPoints;
    }
    void setMaxHitPoints(int value)
    {
        stats.maxHitPoints = value;
    }

    void setMoved(bool value)
    {
        moved = value;
    }
    bool getMoved()
    {
        return moved;
    }

    void setAttacked(bool value)
    {
        attacked = value;
    }
    bool getAttacked()
    {
        return attacked;
    }

    void setMovementData( std::map< std::pair<int, int>, int> value );
    void executeMove(int i, int j);

    void newTurn();

    bool isDone()
    {
        return (moved && attacked);
    }

    //! add an item to the inventory
    void addItem(Item* value);
    void setWeapon(Weapon* value);
    Weapon* getCurrentWeapon();
    std::vector<Technique*> getTechniques()
    {
        return techniques;
    }

    void performAttack(Character* target);
    int simulateAttack(Character* target);

    void printStatus();

    Attack* getCurrentAttack()
    {
        return &currentAttack;
    }
    void prepareWeaponAttack();
    void prepareTechniqueAttack();

    void setCurrentTechnique(int value)
    {
        currentTechnique = techniques[value];
    }

    bool getAI()
    {
        return AI;
    }
    void setAI(bool value)
    {
        AI = value;
    }
    bool isAlive() const;

    virtual void RenderPortrait();
private:
protected:
    std::string name;

    Stats stats;

    bool moved;
    bool attacked;
    //! movement data
    std::map< std::pair<int, int>, int> movementData;

    std::vector<Item*> Inventory;
    Weapon* currentWeapon;

    std::vector<Technique*> techniques;
    Technique* currentTechnique;

    Attack currentAttack;

    bool AI;
};

class CharacterFileLoader : public FileLoader
{
public:
    CharacterFileLoader(const std::string& name) : FileLoader(name) {}
    ~CharacterFileLoader() {}
    void setCharacter(Character* value)
    {
        character = value;
    }
protected:
    Character* character;
    void parseLine(const std::string& field, const std::string& data);
};

#endif

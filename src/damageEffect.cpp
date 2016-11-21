#include "damageEffect.h"
#include "Character.h"
#include "ServiceLocator.h"
#define BOOLEAN_BACKSTAB_BONUS 0.2
#define GRADIENT_BACKSTAB_BONUS 0.15

damageEffect::damageEffect()
{
    //ctor
    setBackstabBonus(GRADIENT);
    setAOE(0);
}

damageEffect::~damageEffect()
{
    //dtor
}

BackstabBonus damageEffect::getBackstabBonus()
{
    return backstabBonus;
}
void damageEffect::setBackstabBonus(BackstabBonus bonus)
{
    backstabBonus = bonus;
}

int damageEffect::getAOE()
{
    return areaOfEffect;
}

void damageEffect::setAOE(int value)
{
    areaOfEffect = value;
}

HeightBonus damageEffect::getHeightBonus()
{
    return heightBonus;
}
void damageEffect::setHeightBonus(HeightBonus bonus)
{
    heightBonus = bonus;
}

int damageEffect::getBaseHealthEffectAmount()
{
    return baseHealthEffectAmount;
}

void damageEffect::setBaseHealthEffectAmount(int value)
{
    baseHealthEffectAmount = value;
}

float damageEffect::calculateBackstabBonus(Character* source, Character* target)
{
    float bonus = 1.f;
    std::string output("Attacking ");
    Directions attackDirection;
    // facing calculations-------------------------
    int aI,aJ,tI,tJ,d;
    aI = source->getI();
    aJ = source->getJ();

    tI = target->getI();
    tJ = target->getJ();
    d = target->getDirection();

    // normalize so that the target is at 0,0
    aI = aI - tI;
    aJ = aJ - tJ;
    // next we rotate until the target is facing east:
    while ( d > 0 )
    {
        Rotate(&aI,&aJ);
        d--;
    }
    // now, we can assume the target is facing east at 0,0
    attackDirection = getDirection(aI,aJ,0,0);

    if (getBackstabBonus() == BS_BOOLEAN)
    {
        bonus = 1 + ( abs(attackDirection-3) > 1 ) * BOOLEAN_BACKSTAB_BONUS;
    }
    else if (getBackstabBonus() == GRADIENT)
    {
        bonus = 1 + abs(attackDirection-3) * GRADIENT_BACKSTAB_BONUS;
    }
// --------------------------------------------
    output += "from the " + std::string(AttackDirectionStrings[attackDirection]) + " for " + std::to_string((int)( 100.f*(bonus) )-100) + "% extra damage.";
    ServiceLocator::getTextLog().addText(output);
    return bonus;
}

float damageEffect::calculateHeightBonus(Character* source, Character* target)
{
    ServiceLocator::getTextLog().addText("Attacker height: " + std::to_string(source->getLevel()) );
    ServiceLocator::getTextLog().addText("Target height: " + std::to_string( target->getLevel()));
    ServiceLocator::getTextLog().addText("Height damage modifer: " + std::to_string( (source->getLevel() - target->getLevel())*getHeightBonus()*10) + "%.");
    return (1 + (source->getLevel() - target->getLevel())/10.0 * getHeightBonus() );
}

int damageEffect::calculateHealthEffectTotal(Character* source, Character* target)
{
    float damage;
    int roundedDamage;

    ServiceLocator::getTextLog().addText("Calculating Damage", textLogColor(1.f,0.f,0.f));
    ServiceLocator::getTextLog().addText("Base damage: " + std::to_string(getBaseHealthEffectAmount()));

    damage = getBaseHealthEffectAmount();
    damage *= calculateBackstabBonus(source, target);
    damage *= calculateHeightBonus(source, target);

    ServiceLocator::getTextLog().addText("Target Damage Reduction: " + std::to_string(target->getDamageReduction()));
    damage -= target->getDamageReduction();

    ServiceLocator::getTextLog().addText("Target Damage Mitigation: " + std::to_string(target->getDamageMitigation()) + "% mitigated");
    damage *= (1 - target->getDamageMitigation() * 1.f/100.f);

    roundedDamage = (int)floor(damage+0.5);
    ServiceLocator::getTextLog().addText("Total damage: " + std::to_string( roundedDamage ));
    return roundedDamage;
}

#include "Attack.h"
#include "Character.h"
#include "HexUtils.h"

void Attack::perform(Character* source, Character* target)
{
    int damageDone = damage.calculateHealthEffectTotal(source, target);
    if (damageDone > 0)
    {
        target->decreaseHitPoints(damageDone);
    }
    else
    {
        target->increaseHitPoints(-damageDone);
    }

    source->incrementTurnCountdown( BASEATTACKDELAY/(float)(source->getSpeed()) );
}

int Attack::calculateDamage(Character* source, Character* target)
{
    return damage.calculateHealthEffectTotal(source,target);
}

bool Attack::checkRange(int i1, int j1, int i2, int j2)
{
    if ( getDistance(i1, j1, i2, j2) <= damage.getAOE() )
    {
        return true;
    }
    return false;
}

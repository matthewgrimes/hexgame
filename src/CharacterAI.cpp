#include "CharacterAI.h"
#include "MainGameState.h"
#include "ServiceLocator.h"
#include <list>

#define NUMDICE 3
#define DICESIDES 3

class Strategy
{
public:
    Strategy() {}
    Strategy(int i, int j, float fitness)
    {
        Fitness = fitness;
        I = i;
        J = j;
    }
    ~Strategy() {}

    float getFitness() const
    {
        return Fitness;
    }
    int getI()
    {
        return I;
    }
    int getJ()
    {
        return J;
    }
    void print();
private:
    float Fitness;
    int I, J;
};

void Strategy::print()
{
    printf("Strategy: %d, %d - Fitness: %f\n",I,J,Fitness);
}


bool compare_Fitness(const Strategy& first, const Strategy& second)
{
    return ( first.getFitness() > second.getFitness() );
}

CharacterAI::CharacterAI()
{
    attack = true;
    initializeGenerator();
    Difficulty = 0;
}

CharacterAI::~CharacterAI()
{

}

void CharacterAI::registerState(MainGameState* value)
{
    state = value;
}

void CharacterAI::populateCharacters()
{
    for ( auto character : state->mCharacters )
    {
        if ( character->getAI() )
        {
            aiCharacters.push_back(character);
        }
        else
        {
            playerCharacters.push_back(character);
        }
    }
}

void CharacterAI::setFacing()
{
    state->activeCharacter->setAngle( getAngle(state->activeCharacter->getI(), state->activeCharacter->getJ(), attackLocation.getI(), attackLocation.getJ() ) );
}

bool CharacterAI::endTurn()
{
    bool done = false;

    done = state->activeCharacter->getMoved();
    done = done && state->activeCharacter->moveList.empty();
    done = done && state->activeCharacter->getAttacked();

    if ( done )
    {
        setFacing();
        attack = true;
    }
    return done;
}

void CharacterAI::performAction()
{
    TextLog& textLog = ServiceLocator::getTextLog();
    ServiceLocator::setTextLog(0);
    populateCharacters();
    if ( state->activeCharacter->getMoved() == false )
    {
        setDestination();
        performMove();
    }
    else if ( state->activeCharacter->getAttacked() == false && state->activeCharacter->moveList.empty() )
    {

        if ( optimizeTechniqueAttack() < 0 && optimizeWeaponAttack() < 0)
        {
            attack = false;
        }
        setAttackLocation();
        ServiceLocator::setTextLog(&textLog);
        if ( attack )
        {
            performAttack();
        }
        else
        {
            state->activeCharacter->setAttacked(true);
        }
    }
    ServiceLocator::setTextLog(&textLog);
}

void CharacterAI::performMove()
{
    state->activeCharacter->executeMove( destination.getI(), destination.getJ() );
    state->activeCharacter->setMoved(true);
}

void CharacterAI::setDestination()
{
    std::list<Strategy> strategyList;
    int i = 0;
    int j = 0;
    float fitness = 0;
    int Isize = 0;
    int Jsize = 0;

    int allyDistanceModifier, enemyDistanceModifier;
    if ( state->activeCharacter->getCurrentWeapon()->getType() == MELEE )
    {
        allyDistanceModifier = 1;
        enemyDistanceModifier = -2;
    }
    else
    {
        allyDistanceModifier = -1;
        enemyDistanceModifier = 2;
    }

    HexCoordinate2D tempPos( state->activeCharacter->getI(), state->activeCharacter->getJ() );

    state->activeCharacter->setMovementData(
        state->currentMap.drawFilledCircle(state->activeCharacter->getI(),
                                           state->activeCharacter->getJ(),
                                           state->activeCharacter->getSpeed(),
                                           false,
                                           state->activeCharacter->getAgility(),
                                           2));



    state->currentMap.getSize(Isize,Jsize);
    HexMapIterator iterator(Isize,Jsize);
    iterator.Reset(i,j);
    while ( iterator.Iterate(i,j) )
    {
        if ( floor( state->currentMap.getCursorData(i,j) ) == 2 )
        {
            state->activeCharacter->setPosition(i,j);
            setAttackLocation();
            fitness = simulateAttack(attackLocation.getI(),attackLocation.getJ());

            fitness += allyDistanceModifier * distanceToAllies(i,j);
            fitness += enemyDistanceModifier * distanceToEnemies(i,j);

            strategyList.push_back(Strategy(i,j,fitness));

        }
    }
    state->currentMap.clearDrawings();

    strategyList.sort(compare_Fitness);

    std::chrono::high_resolution_clock::duration timeElapsed = std::chrono::high_resolution_clock::now() - initTime;
    generator.seed( timeElapsed.count() );

    std::uniform_int_distribution<int> distribution(0,DICESIDES);

    int diceRoll = 0;
    for ( int i = 0; i < NUMDICE; i++)
    {
        diceRoll += distribution(generator);
    }

    diceRoll += getDifficulty();

    for ( int i = getDifficulty(); i < diceRoll; i ++ )
    {
        if ( strategyList.size() > 1 && i >=0 )
        {
            strategyList.pop_front();
        }
    }

    if ( strategyList.front().getFitness() < 0 )
    {
        attack = false;
    }

    destination = HexCoordinate2D( strategyList.front().getI(), strategyList.front().getJ() );

    state->activeCharacter->setPosition(tempPos.getI(),tempPos.getJ());
}

void CharacterAI::setAttackLocation()
{
    int weaponDamage = 0;
    HexCoordinate2D weaponAttackLocation;

    int techniqueDamage = 0;
    HexCoordinate2D techniqueAttackLocation;

    weaponDamage = optimizeWeaponAttack();
    weaponAttackLocation = attackLocation;

    techniqueDamage = optimizeTechniqueAttack();
    techniqueAttackLocation = attackLocation;

    if ( weaponDamage >= techniqueDamage )
    {
        attackLocation = weaponAttackLocation;
        state->activeCharacter->prepareWeaponAttack();
    }
    else
    {
        attackLocation = techniqueAttackLocation;
        state->activeCharacter->setCurrentTechnique(bestTechnique);
        state->activeCharacter->prepareTechniqueAttack();
    }
}

float CharacterAI::optimizeWeaponAttack()
{
    int i = 0;
    int j = 0;
    float damage;
    int Isize, Jsize;
    int bestI = state->activeCharacter->getI();
    int bestJ = state->activeCharacter->getJ();
    float bestDamage = 0;
    std::unordered_map< int, std::unordered_map<int, float> > tempMap;

    state->currentMap.getSize(Isize,Jsize);

    tempMap = state->currentMap.getCursorData();
    state->currentMap.clearDrawings();

    state->activeCharacter->prepareWeaponAttack();
    state->currentMap.drawRing(state->activeCharacter->getI(),
                               state->activeCharacter->getJ(),
                               state->activeCharacter->getCurrentAttack()->getMinRange(),
                               state->activeCharacter->getCurrentAttack()->getMaxRange(), 1);


    HexMapIterator iterator(Isize,Jsize);
    iterator.Reset(i,j);
    while ( iterator.Iterate(i,j) )
    {
        if ( floor( state->currentMap.getCursorData(i,j) ) == 1 )
        {

            damage = simulateAttack(i,j);
            if ( damage > bestDamage )
            {
                bestDamage = damage;
                bestI = i;
                bestJ = j;
            }
        }
    }
    attackLocation = HexCoordinate2D(bestI,bestJ);

    state->currentMap.setCursorData(tempMap);
    return bestDamage;
}

float CharacterAI::optimizeTechniqueAttack()
{
    int i, j, Isize, Jsize;
    float damage;
    int bestI = state->activeCharacter->getI();
    int bestJ = state->activeCharacter->getJ();
    float bestDamage = 0;

    std::unordered_map< int, std::unordered_map<int, float> > tempMap;

    state->currentMap.getSize(Isize,Jsize);

    tempMap = state->currentMap.getCursorData();

    HexMapIterator iterator(Isize,Jsize);

    for ( unsigned int T = 0; T < state->activeCharacter->getTechniques().size(); T++ )
    {
        state->activeCharacter->setCurrentTechnique(T);
        state->activeCharacter->prepareTechniqueAttack();

        state->currentMap.drawRing(state->activeCharacter->getI(), state->activeCharacter->getJ(), state->activeCharacter->getCurrentAttack()->getMinRange(),
                                   state->activeCharacter->getCurrentAttack()->getMaxRange(), 1);
        iterator.Reset(i,j);
        while ( iterator.Iterate(i,j) )
        {
            if ( floor( state->currentMap.getCursorData(i,j) ) == 1 )
            {

                damage = simulateAttack(i,j);
                if ( damage > bestDamage )
                {
                    bestTechnique = T;
                    bestDamage = damage;
                    bestI = i;
                    bestJ = j;
                }

            }


        }
        state->currentMap.clearDrawings();
    }
    attackLocation = HexCoordinate2D(bestI,bestJ);

    state->currentMap.setCursorData(tempMap);
    return bestDamage;
}

float CharacterAI::simulateAttack(int i, int j)
{
    float damage = 0;
    float characterDamage = 0;

    for ( auto character : state->mCharacters )
    {
        if ( state->activeCharacter->getCurrentAttack()->checkRange(i,j,character->getI(),character->getJ()) && abs (state->currentMap.getHeight(i,j) - character->getLevel() ) < 2 )
        {
            characterDamage = 100.f * (1 - 3 * character->getAI() )* state->activeCharacter->simulateAttack(character) * 1.f / character->getMaxHitPoints();
            characterDamage *= ( character->getHitPoints() != 0 );

            damage += characterDamage;
        }
    }
    return damage;
}

void CharacterAI::performAttack()
{
    // loop through the characters to see if one was hit
    for ( auto character : state->mCharacters )
    {
        if ( state->activeCharacter->getCurrentAttack()->checkRange(attackLocation.getI(),attackLocation.getJ(),character->getI(),character->getJ()) &&
                abs (state->currentMap.getHeight(attackLocation.getI(),attackLocation.getJ()) - character->getLevel() ) < 2 )
        {
            state->activeCharacter->performAttack(character);
        }
    }
    state->activeCharacter->setAttacked(true);
    state->currentMap.clearDrawings();
}

float CharacterAI::distanceToAllies(int i, int j)
{
    float distance = 0.f;
    for ( auto ally : aiCharacters )
    {
        distance += getDistance(state->activeCharacter->getI(),state->activeCharacter->getJ(),ally->getI(),ally->getJ());
    }
    distance = distance * 1.f / aiCharacters.size();
    return distance;

}

float CharacterAI::distanceToEnemies(int i, int j)
{
    float distance = 0.f;
    for ( auto ally : playerCharacters )
    {
        distance += getDistance(state->activeCharacter->getI(),state->activeCharacter->getJ(),ally->getI(),ally->getJ());
    }
    distance = distance * 1.f / aiCharacters.size();
    return distance;

}

void CharacterAI::initializeGenerator()
{
    initTime = std::chrono::high_resolution_clock::now();
}

void CharacterAI::setDifficulty(int value)
{
    Difficulty = value;
}

int CharacterAI::getDifficulty()
{
    return Difficulty;
}

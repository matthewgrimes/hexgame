#ifndef CHARACTERAI_H
#define CHARACTERAI_H

#include "HexUtils.h"
#include "Character.h"
#include <vector>
#include <random>
#include <chrono>

class MainGameState;

class CharacterAI
{
public:
    CharacterAI();
    ~CharacterAI();

    void registerState(MainGameState* value);

    void performAction();

    bool endTurn();

    void setDifficulty(int value);
    int getDifficulty();
private:
    MainGameState* state;

    void populateCharacters();
    std::vector<Character*> playerCharacters;
    std::vector<Character*> aiCharacters;

    void performMove();
    void setDestination();
    HexCoordinate2D destination;

    bool attack;
    float simulateAttack(int i, int j);
    void performAttack();
    void setAttackLocation();
    HexCoordinate2D attackLocation;

    float optimizeWeaponAttack();
    float optimizeTechniqueAttack();
    int bestTechnique;

    void setFacing();

    float distanceToAllies(int i, int j);
    float distanceToEnemies(int i, int j);


    std::default_random_engine generator;
    void initializeGenerator();

    std::chrono::high_resolution_clock::time_point initTime;

    int Difficulty;
};

#endif

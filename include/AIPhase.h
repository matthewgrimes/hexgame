#ifndef AIPHASE_H
#define AIPHASE_H

#include "TurnPhase.h"
#include "CharacterAI.h"

class AIPhase : public TurnPhase
{
public:
    AIPhase() : TurnPhase() {}
    ~AIPhase() {}

    void HandleMouseClick();
    void Update();
    void Activate();

    void registerState(MainGameState* value);
private:
    CharacterAI mainAI;

    void endTurn();
};

#endif

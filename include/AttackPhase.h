#ifndef ATTACKPHASE_H
#define ATTACKPHASE_H

#include "TurnPhase.h"

class AttackPhase : public TurnPhase
{

public:
    AttackPhase() : TurnPhase() {}
    ~AttackPhase() {}

    void HandleMouseClick();
    void Update();
    void Activate();
};

#endif

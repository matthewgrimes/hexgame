#ifndef FACINGPHASE_H
#define FACINGPHASE_H

#include "TurnPhase.h"

class FacingPhase : public TurnPhase
{
public:
    FacingPhase() : TurnPhase() {}
    ~FacingPhase() {}

    void HandleMouseClick();
    void Update();
    void Activate();
    void HandleEscape();
};

#endif

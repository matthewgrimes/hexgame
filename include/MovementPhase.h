#ifndef MOVEMENTPHASE_H
#define MOVEMENTPHASE_H

#include "TurnPhase.h"

class MovementPhase : public TurnPhase {
public:
  MovementPhase() : TurnPhase() {}
  ~MovementPhase() {}

  void HandleMouseClick();
  void Update();
  void Activate();
};

#endif

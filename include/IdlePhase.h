#ifndef IDLEPHASE_H
#define IDLEPHASE_H

#include "TurnPhase.h"

class IdlePhase : public TurnPhase {

public:
  IdlePhase() : TurnPhase() {}
  ~IdlePhase() {}

  void HandleMouseClick();
  void Update();
  void Activate();
};

#endif

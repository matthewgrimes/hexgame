#include "TurnPhase.h"
#include "MainGameState.h"

void TurnPhase::registerState(MainGameState *value) { state = value; }

void TurnPhase::HandleEscape() { state->idlePhase.Activate(); }

#include "FacingPhase.h"
#include "MainGameState.h"
#include "ServiceLocator.h"
void FacingPhase::HandleMouseClick() {
  if (floor(state->currentMap.getCursorData(
          state->currentMap.mainCursor.getHexI(),
          state->currentMap.mainCursor.getHexJ())) == 2) {
    state->idlePhase.Activate();
    state->turnOrder.pop_front();
    ServiceLocator::getGUISystem().enableWindow("TurnMenu", "MoveButton");
    ServiceLocator::getGUISystem().enableWindow("TurnMenu", "AttackButton");
    ServiceLocator::getGUISystem().enableWindow("TurnMenu", "TechniqueButton");
    ServiceLocator::getGUISystem().showWindow("TurnMenu");
    state->currentMap.clearDrawings();
  }
}

void FacingPhase::Update() {
  state->activeCharacter->setAngle(
      getAngle(state->activeCharacter->getI(), state->activeCharacter->getJ(),
               state->currentMap.mainCursor.getHexI(),
               state->currentMap.mainCursor.getHexJ()));
  state->currentMap.drawRing(state->activeCharacter->getI(),
                             state->activeCharacter->getJ(), 1, 1, 2);
}

void FacingPhase::HandleEscape() {}

void FacingPhase::Activate() {
  state->currentPhase = this;
  ServiceLocator::getGUISystem().hideWindow("TurnMenu");
}

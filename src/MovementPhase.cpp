#include "MovementPhase.h"
#include "MainGameState.h"
#include "ServiceLocator.h"
void MovementPhase::HandleMouseClick() {
  if (floor(state->currentMap.getCursorData(
          state->currentMap.mainCursor.getHexI(),
          state->currentMap.mainCursor.getHexJ())) == 2) {
    state->activeCharacter->executeMove(state->currentMap.mainCursor.getHexI(),
                                        state->currentMap.mainCursor.getHexJ());

    state->activeCharacter->setMoved(true);
    ServiceLocator::getGUISystem().disableWindow("TurnMenu", "MoveButton");

    state->currentMap.clearDrawings();
  }
}

void MovementPhase::Update() {
  if (state->activeCharacter->getMoved() == true &&
      state->activeCharacter->moveList.empty()) {
    state->idlePhase.Activate();
    ServiceLocator::getGUISystem().disableWindow("TurnMenu", "MoveButton");
    ServiceLocator::getGUISystem().showWindow("TurnMenu");
  }
}

void MovementPhase::Activate() {
  state->currentPhase = this;
  state->activeCharacter->setMovementData(state->currentMap.drawFilledCircle(
      state->activeCharacter->getI(), state->activeCharacter->getJ(),
      state->activeCharacter->getSpeed(), false,
      state->activeCharacter->getAgility(), 2));
  ServiceLocator::getGUISystem().hideWindow("TurnMenu");
}

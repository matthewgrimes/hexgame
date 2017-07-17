#include "IdlePhase.h"
#include "MainGameState.h"
#include "ServiceLocator.h"
void IdlePhase::HandleMouseClick() {
  //! useful variables
  int cI = state->currentMap.mainCursor.getHexI();
  int cJ = state->currentMap.mainCursor.getHexJ();
  if (cI == state->activeCharacter->getI() &&
      cJ == state->activeCharacter->getJ()) {
    ServiceLocator::getGUISystem().showWindow("TurnMenu");
  }
}

void IdlePhase::Update() {
  state->currentMap.setCursorData(state->activeCharacter->getI(),
                                  state->activeCharacter->getJ(), 3);
  state->currentMap.mainCursor.setRadius(0);
}

void IdlePhase::Activate() {
  state->currentPhase = this;

  ServiceLocator::getGUISystem().showWindow("TurnMenu");

  state->currentMap.clearDrawings();
  state->techniqueSelectBox.Hide();
  state->techniqueSelectBox.Cleanup();
}

#include "AttackPhase.h"
#include "MainGameState.h"
#include "ServiceLocator.h"
void AttackPhase::HandleMouseClick() {
  //! useful variables
  int cI = state->currentMap.mainCursor.getHexI();
  int cJ = state->currentMap.mainCursor.getHexJ();

  // check that a red tile was clicked
  if (floor(state->currentMap.getCursorData(cI, cJ)) == 1) {
    // find the target
    // begin declaring it null
    Character *target = NULL;
    // loop through the characters to see if one was hit
    for (auto character : state->mCharacters) {
      // the target now is defined by being in the spot the cursor clicks
      // later on, logic will need to be added for aoe, line of sight, etc.
      if (state->activeCharacter->getCurrentAttack()->checkRange(
              cI, cJ, character->getI(), character->getJ()) &&
          abs(state->currentMap.getHeight(cI, cJ) - character->getLevel()) <
              2) {
        target = character;
        state->activeCharacter->performAttack(character);
      }
    }
    // check if a target was found, otherwise do nothing
    if (target != NULL) {
      state->activeCharacter->setAttacked(true);

      ServiceLocator::getGUISystem().disableWindow("TurnMenu", "AttackButton");
      ServiceLocator::getGUISystem().disableWindow("TurnMenu",
                                                   "TechniqueButton");

      state->activeCharacter->setMoved(true);
      ServiceLocator::getGUISystem().disableWindow("TurnMenu", "MoveButton");

      state->currentMap.clearDrawings();
    }
  }
}

void AttackPhase::Update() {
  // if the character already attacked, move back to idle
  if (state->activeCharacter->getAttacked() == true) {
    state->currentPhase = &state->idlePhase;
    ServiceLocator::getGUISystem().disableWindow("TurnMenu", "AttackButton");
    ServiceLocator::getGUISystem().disableWindow("TurnMenu", "TechniqueButton");
    ServiceLocator::getGUISystem().showWindow("TurnMenu");
  }
  state->activeCharacter->setAngle(
      getAngle(state->activeCharacter->getI(), state->activeCharacter->getJ(),
               state->currentMap.mainCursor.getHexI(),
               state->currentMap.mainCursor.getHexJ()));
}

void AttackPhase::Activate() {
  state->currentPhase = this;

  ServiceLocator::getGUISystem().hideWindow("TurnMenu");

  state->currentMap.drawRing(
      state->activeCharacter->getI(), state->activeCharacter->getJ(),
      state->activeCharacter->getCurrentAttack()->getMinRange(),
      state->activeCharacter->getCurrentAttack()->getMaxRange(), 1);

  state->currentMap.mainCursor.setRadius(
      state->activeCharacter->getCurrentAttack()->getAOE());
  state->techniqueSelectBox.Hide();
}

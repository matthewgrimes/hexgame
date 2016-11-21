#include "AIPhase.h"
#include "MainGameState.h"
#include "ServiceLocator.h"
void AIPhase::HandleMouseClick()
{

}

void AIPhase::Update()
{
    mainAI.performAction();

    if ( mainAI.endTurn() )
    {
        endTurn();
    }
}

void AIPhase::Activate()
{
    state->currentPhase = this;
    ServiceLocator::getGUISystem().showWindow("TurnMenu");
}

void AIPhase::registerState(MainGameState* value)
{
    TurnPhase::registerState(value);
    mainAI.registerState(value);
}

void AIPhase::endTurn()
{
    state->idlePhase.Activate();
    state->turnOrder.pop_front();
    ServiceLocator::getGUISystem().enableWindow("TurnMenu", "MoveButton");
    ServiceLocator::getGUISystem().enableWindow("TurnMenu", "AttackButton");
    ServiceLocator::getGUISystem().enableWindow("TurnMenu", "TechniqueButton");
    ServiceLocator::getGUISystem().showWindow("TurnMenu");
    state->currentMap.clearDrawings();
}

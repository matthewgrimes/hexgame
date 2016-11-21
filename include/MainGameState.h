#ifndef MAINGAMESTATE_H
#define MAINGAMESTATE_H

#include "CGameState.h"
#include "HexMap.h"
#include "Camera.h"
#include "Character.h"
#include "TextLog.h"
#include "TechniqueSelect.h"
#include "CharacterInfoBox.h"
#include "FacingPhase.h"
#include "MovementPhase.h"
#include "AttackPhase.h"
#include "IdlePhase.h"
#include "AIPhase.h"
#include <deque>

class MainGameState : public CGameState
{
    friend class TurnPhase;
    friend class FacingPhase;
    friend class MovementPhase;
    friend class AttackPhase;
    friend class IdlePhase;
    friend class AIPhase;
    friend class CharacterAI;
public:
    virtual void Init(); /** Constructor */
    virtual void Cleanup();

    virtual void Pause();
    virtual void Resume();

    virtual void HandleEvents(CGameEngine* game);
    virtual void Update(CGameEngine* game);
    virtual void Draw(CGameEngine* game);

    static MainGameState* Instance()
    {
        return &m_MainGameState;
    }


protected:
    MainGameState() {};

    HexMap currentMap;

    Camera mainCamera;

    std::deque<Character*> mCharacters;
    Character* activeCharacter;
    std::deque<Character*> turnOrder;

    TechniqueSelect techniqueSelectBox;

    CharacterInfoBox characterInfo;

    void resetToIdle();

    void drawPortraits();

    TurnPhase* currentPhase;
    FacingPhase facingPhase;
    MovementPhase movementPhase;
    AttackPhase attackPhase;
    IdlePhase idlePhase;
    AIPhase aiPhase;
private:
    static MainGameState m_MainGameState;

    void HandleKeyEvent( SDL_Event event );
    void HandleMouseEvent( SDL_Event event);
};

#endif // MAINGAMESTATE_H

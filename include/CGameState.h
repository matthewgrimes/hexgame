#ifndef CGAMESTATE_H
#define CGAMESTATE_H

#include "CGameEngine.h"

//! Class for handling game states

class CGameState
{
public:
    virtual void Init() = 0;
    virtual void Cleanup() = 0;

    virtual void Pause() = 0;
    virtual void Resume() = 0;

    virtual void HandleEvents(CGameEngine* game) = 0;
    virtual void Update(CGameEngine* game) = 0;
    virtual void Draw(CGameEngine* game) = 0;

protected:
    CGameState() { }
};

#endif // CGAMESTATE_H

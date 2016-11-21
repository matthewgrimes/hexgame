#ifndef TURNPHASE_H
#define TURNPHASE_H

class MainGameState;

class TurnPhase
{
public:
    TurnPhase() {}
    ~TurnPhase() {}

    virtual void HandleMouseClick()=0;
    virtual void Update()=0;
    virtual void Activate()=0;
    virtual void HandleEscape();

    virtual void registerState(MainGameState* value);
protected:
    MainGameState* state;
};

#endif

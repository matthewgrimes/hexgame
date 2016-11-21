#ifndef BATTLEMAPSTATE_H
#define BATTLEMAPSTATE_H

#include <CGameState.h>


class BattleMapState : public CGameState
{
public:
    virtual ~BattleMapState();

    virtual void Init();
    virtual void Cleanup();

    virtual void Pause();
    virtual void Resume();

    virtual void HandleEvents(GameManager* game);
    virtual void Update(GameManager* game);
    virtual void Draw(GameManager* game);

    static BattleMapState* Instance();

protected:
    BattleMapState();

private:
    static BattleMapState m_Instance;
};

#endif // BATTLEMAPSTATE_H

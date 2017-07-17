#ifndef MainMenuState_H
#define MainMenuState_H

#include "GLText.h"

#include <GL/glew.h>

#include "CGameState.h"
#include "HexGameEngine.h"
//! Basic game state to serve as template
class MainMenuState : public CGameState {
public:
  void Init(); /** Constructor */
  void Cleanup();

  void Pause();
  void Resume();

  void HandleEvents(CGameEngine *game);
  void Update(CGameEngine *game);
  void Draw(CGameEngine *game);

  static MainMenuState *Instance() { return &m_MainMenuState; }

protected:
  MainMenuState() {}

private:
  static MainMenuState m_MainMenuState;
  CGameEngine *m_Game;
};

#endif // MainMenuState_H

#ifndef MAPEDITORSTATE_H
#define MAPEDITORSTATE_H

#include "CGameState.h"
#include "Camera.h"
#include "HexMap.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
class MapEditorState : public CGameState {
public:
  virtual void Init(); /** Constructor */
  virtual void Cleanup();

  virtual void Pause();
  virtual void Resume();

  virtual void HandleEvents(CGameEngine *game);
  virtual void Update(CGameEngine *game);
  virtual void Draw(CGameEngine *game);

  static MapEditorState *Instance() { return &m_MapEditorState; }

protected:
  MapEditorState(){};

private:
  static MapEditorState m_MapEditorState;

  HexMap currentMap;

  Camera mainCamera;

  CEGUI::WindowManager *Wmgr;
  CEGUI::Window *rootWindow;

  int currentTile;
  void drawCurrentTile();

  int currentLayer;
};

#endif

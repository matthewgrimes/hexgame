#include "MapEditorState.h"
#include "HexUtils.h"
#include "ServiceLocator.h"
MapEditorState MapEditorState::m_MapEditorState;

void MapEditorState::Init() {

  currentTile = 1;
  currentLayer = 0;

  mainCamera.Init();

  // load from file
  currentMap.loadFromFile("data/maps/empty.map");
  currentMap.Init();

  // CEGUI
  Wmgr = CEGUI::WindowManager::getSingletonPtr();
  rootWindow =
      CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

  currentMap.mainCursor.Deactivate();
}

void MapEditorState::Cleanup() {
  // dtor
}

void MapEditorState::Pause() { rootWindow->getChild("TurnMenu")->hide(); }

void MapEditorState::Resume() { rootWindow->getChild("TurnMenu")->show(); }

void MapEditorState::HandleEvents(CGameEngine *game) {
  //! useful variables
  int cI = currentMap.mainCursor.getHexI();
  int cJ = currentMap.mainCursor.getHexJ();
  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    // First, pass the event to CEGUI
    ServiceLocator::getGUISystem().HandleEvent(event);

    // now handle event
    if (event.type == SDL_QUIT) {
      game->Quit();
    }
    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        break;

      case SDLK_SPACE:
        currentLayer++;
        if (currentLayer >= currentMap.getNumLayers()) {
          currentLayer = 0;
        }
        break;
      case SDLK_TAB:
        currentTile++;
        if (currentTile >= currentMap.getCurrentTileSetSize()) {
          currentTile = 1;
        }
        break;
      case SDLK_h:

        break;

      case SDLK_g:
        currentMap.toggleGrid();
        break;

      case SDLK_w:
        mainCamera.SetspeedY(500.0);
        break;

      case SDLK_s:
        if (event.key.keysym.mod == KMOD_LCTRL) {
          currentMap.Save();
        } else {
          mainCamera.SetspeedY(-500.0);
        }
        break;

      case SDLK_a:
        mainCamera.SetspeedX(500.0);
        break;

      case SDLK_d:
        mainCamera.SetspeedX(-500.0);
        break;

      case SDLK_q:
        mainCamera.Rotate(30, 0, 0, 1);
        break;

      case SDLK_e:
        mainCamera.Rotate(-30, 0, 0, 1);
        break;

      // viewport functions
      case SDLK_p:
        break;
      case SDLK_DOWN:
        break;
      case SDLK_KP_PLUS:
        break;
      default:
        break;
      }
    }
    if (event.type == SDL_KEYUP) {
      switch (event.key.keysym.sym) {
      case SDLK_w:
        mainCamera.SetspeedY(0.0);
        break;

      case SDLK_s:
        mainCamera.SetspeedY(0.0);
        break;

      case SDLK_a:
        mainCamera.SetspeedX(0.0);
        break;

      case SDLK_d:
        mainCamera.SetspeedX(0.0);
        break;
      case SDLK_r:
        break;
      default:
        break;
      }
    }
    if (event.type == SDL_MOUSEWHEEL) {
      if (event.wheel.y != 0) {
        mainCamera.changeZoom(pow(1.05, event.wheel.y));
      }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        currentMap.setCurrentData(cI, cJ, currentLayer, currentTile);
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        currentMap.setCurrentData(cI, cJ, currentLayer,
                                  (int)(currentLayer == 0));
      }
    }

    if (event.type == SDL_MOUSEBUTTONUP) {
    }
  }
}

void MapEditorState::Update(CGameEngine *game) {
  mainCamera.Update();

  currentMap.mainCursor.Activate();
  currentMap.clearActorLocations();

  if (CEGUI::System::getSingleton()
          .getDefaultGUIContext()
          .getWindowContainingMouse()
          ->getName() == "TextLog") {
    currentMap.mainCursor.Deactivate();
  }
}

void MapEditorState::Draw(CGameEngine *game) {
  // prepare matrices
  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_DEPTH_TEST);

  // save camera matrix
  glPushMatrix();

  // enable lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  GLfloat lightpos[] = {0., 0., 100., 0.};
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_BLEND);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  currentMap.Draw();

  glPopMatrix();

  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  drawCurrentTile();
}

void MapEditorState::drawCurrentTile() {
  glPushMatrix();
  glLoadIdentity();
  glScalef(.5f, .5f, .5f);
  /*HexCoordinate3D topLeft = convertScreenPositionToScene(0, 0, 0, 0, 0);
  topLeft.setX(topLeft.getX() + TILEWIDTH);
  topLeft.setY(topLeft.getY() + TILERADIUS);
  glTranslatef(topLeft.getX(),topLeft.getY(),0.f);*/
  currentMap.getTileset()->getMember(currentTile)->Draw();
  glPopMatrix();
}

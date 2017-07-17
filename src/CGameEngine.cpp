#include "CGameEngine.h"
#include "CEGUISystem.h"
#include "CGameState.h"
#include "MainGameState.h"
#include "MapEditorState.h"
#include "Pipeline.h"
#include "PrintfTextLog.h"
#include "ServiceLocator.h"
#include "ShaderManager.h"
#include "Version.h"
#include <fstream>
#include <string>

class CGameState;

CGameEngine::CGameEngine() { m_running = false; }
CGameEngine::~CGameEngine() {}

void CGameEngine::Init(const char *title, CGameState *initialState) {
  PrintfTextLog *logger = new PrintfTextLog;
  ServiceLocator::setTextLog(logger);
  // parse the file hexgame.cfg for info like bpp width height and fullscreen
  // int bpp;
  int width = 0;
  int height = 0;
  bool fullscreen = false;
  std::string line;
  std::ifstream file(std::string("data/hexgame.cfg"));

  std::string field;
  if (file.is_open()) {
    std::size_t spacemarker;
    ServiceLocator::getTextLog().addText("=======LOADING CONFIG=======\n");
    // get the next line
    while (std::getline(file, line)) {
      // only parse if the line isn't a comment
      if (line.compare(0, 1, "#") != 0) {
        spacemarker = line.find_first_of(" ");
        field = line.substr(0, spacemarker);

        if (field.compare("width") == 0) {
          width = atoi(line.substr(spacemarker + 1).c_str());
          ServiceLocator::getTextLog().addText("Width: " +
                                               std::to_string(width));
        } else if (field.compare("height") == 0) {
          std::string heightstring = line.substr(spacemarker + 1);
          height = atoi(heightstring.c_str());
          ServiceLocator::getTextLog().addText("Height: " + heightstring);
        } else if (field.compare("fullscreen") == 0) {
          fullscreen = (bool)atoi(line.substr(spacemarker + 1).c_str());
          // ServiceLocator::getTextLog().addText("Fullscreen:
          // %d\n",fullscreen);
        }
      }
    }
    ServiceLocator::getTextLog().addText("Loading complete.\n");
    file.close();
  }
  //-------------------------------------------------------------------

  std::string windowName;
  windowName += "Hexagonal Tactics | ";
  windowName += std::string(HexGame_VERSION_SHORT);
  SDLWindowInfo displayInfo = SDLWindowInfo(windowName.c_str(), width, height);

  ServiceLocator::setMainDisplay(new SDLDisplay);
  ServiceLocator::getMainDisplay().Initialize(displayInfo);
  ServiceLocator::getMainDisplay().getShaderManager()->initFBO(width, height);
  ServiceLocator::getMainDisplay().getShaderManager()->Activate("standard");

  // Initialize CEGUI
  CEGUISystem *guiSystem = new CEGUISystem;
  guiSystem->Initialize();
  ServiceLocator::setGUISystem(guiSystem);
  ServiceLocator::getGUISystem().addRootFromFile("MainMenuState");

  ServiceLocator::getGUISystem().addChildFromFile("textfeed.layout");
  // TOGGLE FPS COUNTER
  // fpsCounter->hide();

  ServiceLocator::getGUISystem().addChildFromFile("MainMenu.layout");
  ServiceLocator::getGUISystem().subscribeFunction("MainMenu", "Quit",
                                                   [this]() { this->Quit(); });
  ServiceLocator::getGUISystem().subscribeFunction(
      "MainMenu", "NewGame",
      [this]() { this->PushState(MainGameState::Instance()); });
  ServiceLocator::getGUISystem().subscribeFunction(
      "MainMenu", "MapEditor",
      [this]() { this->PushState(MapEditorState::Instance()); });
  ServiceLocator::getGUISystem().hideWindow("MainMenu");

  // ------------------------------
  //    m_fullscreen = fullscreen;
  m_running = true;
  // Load the template state
  ChangeState(initialState);

  // Main Loop
  int frames = 0;
  m_FrameTimer.Start();
  m_MainGameTimer.Start();
  while (Running()) {
    HandleEvents();
    Update();

    Draw();
    frames++;
    if (m_FrameTimer.getSecondsElapsed() > 1) {
      float fps = (float)(frames) / (m_FrameTimer.getSecondsElapsed());
      std::string fpsString = "mSPF: " + std::to_string(1. / fps * 1000);
      ServiceLocator::getGUISystem().setProperty("textfeed", "Text", fpsString);
      frames = 0;
      m_FrameTimer.Reset();
    }
  }
  Cleanup();
  delete logger;
}

void CGameEngine::Cleanup() {
  while (!states.empty()) {
    states.back()->Cleanup();
    states.pop_back();
  }

  ServiceLocator::Cleanup();

  ShaderManager::getInstance()->Cleanup();
  //    SDL_GL_DeleteContext(mainGLContext);
  //    SDL_DestroyWindow( sdlWindow );
  // mainDisplay.Cleanup();
  SDL_Quit();
}

void CGameEngine::PushState(CGameState *state) {
  // pause current state
  if (!states.empty()) {
    states.back()->Pause();
  }

  // store and init the new state
  states.push_back(state);
  states.back()->Init();
}

void CGameEngine::PopState() {
  // cleanup the current state
  if (!states.empty()) {
    states.back()->Cleanup();
    states.pop_back();
  }

  // resume previous state
  if (!states.empty()) {
    states.back()->Resume();
  }
}

CGameState *CGameEngine::GetCurrentState() { return states.back(); }

void CGameEngine::ChangeState(CGameState *State) {
  // cleanup old state
  if (!states.empty()) {
    states.back()->Cleanup();
    states.pop_back();
  }

  // store and init the new state
  states.push_back(State);
  states.back()->Init();
}

void CGameEngine::HandleEvents() {
  if (states.empty())
    return;
  states.back()->HandleEvents(this);
}

void CGameEngine::Update() {
  ServiceLocator::getGUISystem().injectDeltaTime(
      m_MainGameTimer.getSecondsElapsed());
  m_MainGameTimer.Reset();

  states.back()->Update(this);
}

void CGameEngine::Draw() {
  ServiceLocator::getMainDisplay().setupDraw();

  ServiceLocator::getMainDisplay().setupSceneRender();

  ServiceLocator::getMainDisplay().getShaderManager()->setUniform(
      Pipeline::getInstance()->GetWorldTrans(), "gWorld");
  ServiceLocator::getMainDisplay().getShaderManager()->setUniform(
      Pipeline::getInstance()->getCameraTarget(), "cameraDirection");

  states.back()->Draw(this);

  ServiceLocator::getMainDisplay().setupPickingRender();

  ServiceLocator::getMainDisplay().getShaderManager()->setUniform(
      Pipeline::getInstance()->GetWorldTrans(), "gWorld");
  ServiceLocator::getMainDisplay().getShaderManager()->setUniform(
      Pipeline::getInstance()->getCameraTarget(), "cameraDirection");

  states.back()->Draw(this);
  ServiceLocator::getMainDisplay().finishPickingRender();

  ServiceLocator::getMainDisplay().setupGUIRender();

  ServiceLocator::getGUISystem().Render();

  ServiceLocator::getMainDisplay().finishDraw();
}

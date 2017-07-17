#include "MainGameState.h"
#include "CEGUITextLog.h"
#include "GLOBALS.h"
#include "Pipeline.h"
#include "ServiceLocator.h"
#include <fstream>
#include <sstream>

MainGameState MainGameState::m_MainGameState;

void MainGameState::Init() {

  mainCamera.Init();

  ServiceLocator::setTextLog(new CEGUITextLog);
  ServiceLocator::getTextLog().Init();

  facingPhase.registerState(this);
  movementPhase.registerState(this);
  attackPhase.registerState(this);
  idlePhase.registerState(this);
  aiPhase.registerState(this);

  mCharacters.clear();
  // load from file
  std::string line;
  std::ifstream file("data/saves/save001.sav");

  std::string field;
  if (file.is_open()) {
    ServiceLocator::getTextLog().addText("Loading savefile.",
                                         textLogColor(0.f, 1.f, 1.f));

    while (std::getline(file, line)) {
      if (line.compare(0, 1, "#") != 0) {
        std::size_t spacemarker = line.find_first_of(" ");
        field = line.substr(0, spacemarker);
        if (field.compare("character") == 0) {
          int i, j, direction;
          field = line.substr(spacemarker + 1);
          spacemarker = field.find_first_of(" ");

          mCharacters.push_back(new Character);
          mCharacters.back()->setTurnCountdown(100);

          // printf("%s\n",field.substr(0,spacemarker).c_str());
          mCharacters.back()->loadFromFile(field.substr(0, spacemarker));
          std::istringstream iss(field.substr(spacemarker + 1));
          iss >> i;
          iss >> j;
          iss >> direction;
          mCharacters.back()->setPosition(i, j);
          mCharacters.back()->setLevel(currentMap.getHeight(i, j));
          mCharacters.back()->setDirection(direction);
          currentMap.addActorLocation(i, j);
        } else if (field.compare("map") == 0) {
          currentMap.loadFromFile(line.substr(spacemarker + 1).c_str());
          currentMap.Init();
        }
      }
    }

    ServiceLocator::getTextLog().addText("Loading Complete.");
    file.close();
  }

  turnOrder.clear();

  ServiceLocator::getGUISystem().addChildFromFile("TurnMenu.layout");

  ServiceLocator::getGUISystem().subscribeFunction(
      "TurnMenu", "MoveButton", [this]() { this->movementPhase.Activate(); });
  ServiceLocator::getGUISystem().subscribeFunction(
      "TurnMenu", "AttackButton", [this]() {
        this->activeCharacter->prepareWeaponAttack();
        this->attackPhase.Activate();
      });
  ServiceLocator::getGUISystem().subscribeFunction(
      "TurnMenu", "TechniqueButton", [this]() {
        this->techniqueSelectBox.registerCharacter(activeCharacter);
        this->techniqueSelectBox.Show();
        ServiceLocator::getGUISystem().hideWindow("TurnMenu");
      });
  ServiceLocator::getGUISystem().subscribeFunction(
      "TurnMenu", "BackButton",
      []() { ServiceLocator::getGUISystem().hideWindow("TurnMenu"); });
  ServiceLocator::getGUISystem().subscribeFunction(
      "TurnMenu", "EndTurnButton", [this]() { this->facingPhase.Activate(); });
  techniqueSelectBox.Initialize();
  techniqueSelectBox.Hide();

  techniqueSelectBox.registerActions(
      [this]() { this->resetToIdle(); },
      [this]() {
        this->activeCharacter->prepareTechniqueAttack();
        this->attackPhase.Activate();
      });

  characterInfo.Initialize();
  characterInfo.Hide();

  idlePhase.Activate();
  currentMap.mainCursor.Deactivate();
}

void MainGameState::Cleanup() {
  // dtor
  for (auto actor : mCharacters) {
    delete actor;
  }
  mCharacters.clear();
  activeCharacter = NULL;
  // rootWindow->destroyChild("TurnMenu");
}

void MainGameState::Pause() {
  ServiceLocator::getGUISystem().hideWindow("TurnMenu");
}

void MainGameState::Resume() {
  ServiceLocator::getGUISystem().showWindow("TurnMenu");
}

void MainGameState::HandleEvents(CGameEngine *game) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ServiceLocator::getGUISystem().HandleEvent(event);

    if (event.type == SDL_QUIT) {
      game->Quit();
    }
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
      HandleKeyEvent(event);
    }
    if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP ||
        event.type == SDL_MOUSEWHEEL) {
      HandleMouseEvent(event);
    }
  }
}

void MainGameState::HandleKeyEvent(SDL_Event event) {
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
      currentPhase->HandleEscape();
      break;

    case SDLK_h:

      break;

    case SDLK_TAB:
      break;

    case SDLK_g:
      currentMap.toggleGrid();
      break;

    case SDLK_w:
      mainCamera.SetspeedY(5.0);
      break;

    case SDLK_s:
      mainCamera.SetspeedY(-5.0);
      break;

    case SDLK_a:
      mainCamera.SetspeedX(-5.0);
      break;

    case SDLK_d:
      mainCamera.SetspeedX(5.0);
      break;

    case SDLK_q:
      mainCamera.Rotate(30, 0, 0, 1);
      break;

    case SDLK_e:
      mainCamera.Rotate(-30, 0, 0, 1);
      break;

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
    case SDLK_TAB:
      break;

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
}

void MainGameState::HandleMouseEvent(SDL_Event event) {
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      currentPhase->HandleMouseClick();
    } else if (event.button.button == SDL_BUTTON_RIGHT) {
      resetToIdle();
    }
  }
  if (event.type == SDL_MOUSEWHEEL) {
    if (event.wheel.y != 0) {
      mainCamera.changeZoom(pow(1.05, event.wheel.y));
    }
  }
}

void MainGameState::Update(CGameEngine *game) {
  std::sort(mCharacters.begin(), mCharacters.end(),
            Actor::turnOrderCompareFunction);
  if (!turnOrder.empty()) {
    activeCharacter = turnOrder.front();
    while (!turnOrder.empty() && activeCharacter->getHitPoints() <= 0) {
      turnOrder.pop_front();
      if (!turnOrder.empty()) {
        activeCharacter = turnOrder.front();
      }
    }
  }
  if (turnOrder.empty()) {
    /** new turn stuff */
    for (auto character : mCharacters) {
      // insert the character into the queue
      if (character->getHitPoints() > 0) {
        turnOrder.push_back(character);
      }
    }

    std::sort(turnOrder.begin(), turnOrder.end(),
              Actor::turnOrderCompareFunction);

    /* keep delays from growing too big */
    int smallestCountdown = turnOrder.front()->getTurnCountdown();
    for (auto character : mCharacters) {
      character->setTurnCountdown(character->getTurnCountdown() -
                                  smallestCountdown);
    }

    // printf("Upcoming turns:\n");
    // printf("Now: %s -
    // %f\n",turnOrder.front()->getName().c_str(),turnOrder.front()->getTurnCountdown());
    // for ( unsigned int i = 1; i < turnOrder.size(); i ++ )
    //{
    // printf("In %u turn(s): %s - %f\n",i,turnOrder[i]->getName().c_str(),
    // turnOrder[i]->getTurnCountdown());
    //}
    ServiceLocator::getGUISystem().enableWindow("TurnMenu", "MoveButton");
    ServiceLocator::getGUISystem().enableWindow("TurnMenu", "AttackButton");
    ServiceLocator::getGUISystem().enableWindow("TurnMenu", "TechniqueButton");
    ServiceLocator::getGUISystem().showWindow("TurnMenu");

    /** done with new turn stuff */
    while (turnOrder.size() > 1) {
      turnOrder.pop_back();
    }
    activeCharacter = turnOrder.front();
    activeCharacter->newTurn();
  }
  if (activeCharacter->getAI()) {
    aiPhase.Activate();
  }

  // activeCharacter is now equal to turnOrder.front()
  if (activeCharacter->getTechniques().empty()) {
    ServiceLocator::getGUISystem().disableWindow("TurnMenu", "TechniqueButton");
  }

  currentPhase->Update();

  characterInfo.Hide();

  mainCamera.Update();

  currentMap.mainCursor.Activate();
  currentMap.clearActorLocations();
  currentMap.mainCursor.setPosition(currentMap.getMouseTile());

  if (ServiceLocator::getGUISystem().getNameOfWindowContainingMouse() ==
      "TextLog") {
    currentMap.mainCursor.Deactivate();
  }
  if (activeCharacter->getAI()) {
    currentMap.mainCursor.Deactivate();
  }
  for (auto actor : mCharacters) {
    actor->Update();

    actor->setLevel(currentMap.getHeight(actor->getI(), actor->getJ()));
    currentMap.addActorLocation(actor->getI(), actor->getJ());
    if (actor->isMoving()) {
      currentMap.mainCursor.Deactivate();
    }
    if (currentMap.mainCursor.getHexI() == actor->getI() &&
        currentMap.mainCursor.getHexJ() == actor->getJ()) {
      characterInfo.registerCharacter(actor);
      characterInfo.Show();
    }
  }
}

void MainGameState::Draw(CGameEngine *game) {

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  // glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glBlendFunc(GL_ZERO, GL_SRC_COLOR);
  currentMap.Draw();

  for (auto actor : mCharacters) {
    glUniform1i(
        glGetUniformLocation(ShaderManager::getInstance()->getProgram(), "gI"),
        actor->getHexCoordIJ().getI() +
            (actor->getHexCoordIJ().getJ() -
             actor->getHexCoordIJ().getJ() % 2) /
                2);
    glUniform1i(
        glGetUniformLocation(ShaderManager::getInstance()->getProgram(), "gJ"),
        actor->getHexCoordIJ().getJ());
    glUniform1i(glGetUniformLocation(ShaderManager::getInstance()->getProgram(),
                                     "gLayer"),
                actor->getLevel() + 1);
    actor->Render();
  }
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  Frame tempFrame = mainCamera.getFrame();

  drawPortraits();
}
void MainGameState::drawPortraits() {
  int j = mCharacters.size() - 1;
  float offset = 2.5f;
  mainCamera.Push();
  mainCamera.setPos(2.f, 40.f, 0.f);
  for (unsigned int i = 0; i < mCharacters.size(); i++) {

    float vertOffset = .2f * (activeCharacter == mCharacters[j]);
    if (mCharacters[mCharacters.size() - 1 - i]->isAlive()) {
      glUniform1i(
          glGetUniformLocation(ShaderManager::getInstance()->getProgram(),
                               "gI"),
          mCharacters[mCharacters.size() - 1 - i]->getHexCoordIJ().getI() +
              (mCharacters[mCharacters.size() - 1 - i]->getHexCoordIJ().getJ() -
               mCharacters[mCharacters.size() - 1 - i]->getHexCoordIJ().getJ() %
                   2) /
                  2);
      glUniform1i(
          glGetUniformLocation(ShaderManager::getInstance()->getProgram(),
                               "gJ"),
          mCharacters[mCharacters.size() - 1 - i]->getHexCoordIJ().getJ());
      glUniform1i(glGetUniformLocation(
                      ShaderManager::getInstance()->getProgram(), "gLayer"),
                  mCharacters[mCharacters.size() - 1 - i]->getLevel() + 1);

      vertOffset +=
          .1f * (currentMap.mainCursor.getHexCoordIJ() ==
                 mCharacters[mCharacters.size() - 1 - i]->getHexCoordIJ());
      Pipeline::getInstance()->setObjectPos(
          Vector3f(j * 2.5f + offset, 0.f, 9.7f - vertOffset));
      mCharacters[mCharacters.size() - 1 - i]->RenderPortrait();
      j--;
    }
  }
  mainCamera.Pop();
}

void MainGameState::resetToIdle() { idlePhase.Activate(); }

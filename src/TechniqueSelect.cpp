#include "TechniqueSelect.h"

void TechniqueSelect::Initialize() {
  rootWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(
      "TechniqueSelect.layout");

  CEGUI::System::getSingleton()
      .getDefaultGUIContext()
      .getRootWindow()
      ->addChild(rootWindow);
}

void TechniqueSelect::registerActions(
    std::function<void(void)> cancelAction,
    std::function<void(void)> continueAction) {
  auto CEGUIcancelAction = [cancelAction](const CEGUI::EventArgs &e) {
    cancelAction();
    return true;
  };
  auto CEGUIcontinueAction = [continueAction](const CEGUI::EventArgs &e) {
    continueAction();
    return true;
  };

  rootWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,
                             CEGUIcancelAction);
  rootWindow->subscribeEvent(
      CEGUI::FrameWindow::EventCloseClicked,
      CEGUI::Event::Subscriber(&TechniqueSelect::CleanupList, this));

  rootWindow->getChild("SelectButton")
      ->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUIcontinueAction);
  rootWindow->getChild("SelectButton")
      ->subscribeEvent(
          CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&TechniqueSelect::CleanupList, this));
}

void TechniqueSelect::registerCharacter(Character *character) {
  m_character = character;
  int i = 0;
  CEGUI::RadioButton *item;
  for (auto tech : m_character->getTechniques()) {
    item = static_cast<CEGUI::RadioButton *>(
        CEGUI::WindowManager::getSingleton().createWindow(
            "TaharezLook/RadioButton", std::to_string(i)));
    if (tech->getDamage() > 0) {
      item->setText(tech->getName() + " Damage: " +
                    std::to_string(tech->getDamage()) + " AOE: " +
                    std::to_string(tech->getAOE()));
    } else {
      item->setText(tech->getName() + " Healing: " +
                    std::to_string(-tech->getDamage()) + " AOE: " +
                    std::to_string(tech->getAOE()));
    }
    item->setProperty("Position", "{{0,0},{0," + std::to_string(i * 32) + "}}");
    item->setGroupID(0);
    rootWindow->getChild("ScrollablePane")->addChild(item);
    if (i == 0) {
      item->subscribeEvent(
          CEGUI::RadioButton::EventSelectStateChanged,
          CEGUI::Event::Subscriber(&TechniqueSelect::SelectionChanged, this));
      item->setSelected(true);
    }
    i++;
  }
  adjustWindowSize();
}
bool TechniqueSelect::SelectionChanged(const CEGUI::EventArgs &e) {
  CEGUI::RadioButton *currentButton =
      static_cast<CEGUI::RadioButton *>(
          rootWindow->getChild("ScrollablePane")->getChild("0"))
          ->getSelectedButtonInGroup();
  int index = atoi(currentButton->getName().c_str());
  m_character->setCurrentTechnique(index);
  return 1;
}

bool TechniqueSelect::CleanupList(const CEGUI::EventArgs &e) {
  Cleanup();
  return true;
}

void TechniqueSelect::Cleanup() {
  if (m_character) {
    for (unsigned int i = 0; i < m_character->getTechniques().size(); i++) {
      if (rootWindow->getChild("ScrollablePane")->isChild(std::to_string(i))) {
        rootWindow->getChild("ScrollablePane")
            ->getChild(std::to_string(i))
            ->destroy();
      }
    }
  }
}

void TechniqueSelect::adjustWindowSize() {
  rootWindow->setProperty("Area", "{{0,0},{0,0},{0.3,12},{0.3,134}}");
}
void TechniqueSelect::Hide() {
  rootWindow->hide();
  Cleanup();
}

void TechniqueSelect::Show() { rootWindow->show(); }

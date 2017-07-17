#include "CharacterInfoBox.h"

void CharacterInfoBox::Initialize() {
  rootWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(
      "CharacterInfo.layout");

  CEGUI::System::getSingleton()
      .getDefaultGUIContext()
      .getRootWindow()
      ->addChild(rootWindow);
}

void CharacterInfoBox::Cleanup() {
  static_cast<CEGUI::Listbox *>(rootWindow->getChild("Listbox"))->resetList();
}

void CharacterInfoBox::registerCharacter(Character *character) {
  std::string output;
  m_character = character;

  output = m_character->getName();
  addText(output);

  output = "HP: " + std::to_string(m_character->getHitPoints()) + " / " +
           std::to_string(m_character->getMaxHitPoints());
  addText(output);

  output = "Speed: " + std::to_string(m_character->getSpeed());
  addText(output);

  output = "Countdown: " + std::to_string(m_character->getTurnCountdown());
  addText(output);

  output = "Equipped Weapon: " + m_character->getCurrentWeapon()->getName();
  addText(output);

  output = "Range: " +
           std::to_string(m_character->getCurrentWeapon()->getMinRange()) +
           "-" + std::to_string(m_character->getCurrentWeapon()->getMaxRange());
  addText(output);

  output =
      "Damage: " + std::to_string(m_character->getCurrentWeapon()->getDamage());
  addText(output);

  for (auto tech : m_character->getTechniques()) {
    output = "Technique: " + tech->getName();
    addText(output);

    output = "    Damage: " + std::to_string(tech->getDamage());
    addText(output);
    output = "    AOE: " + std::to_string(tech->getAOE());
    addText(output);
  }
}

void CharacterInfoBox::Hide() {
  rootWindow->hide();
  Cleanup();
}

void CharacterInfoBox::Show() { rootWindow->show(); }

void CharacterInfoBox::addText(std::string text) {
  static_cast<CEGUI::Listbox *>(rootWindow->getChild("Listbox"))
      ->addItem(new CEGUI::ListboxTextItem(text));
}

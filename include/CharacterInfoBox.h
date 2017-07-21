#ifndef CHARACTERINFOBOX_H
#define CHARACTERINFOBOX_H

#include "Character.h"
#include <CEGUI/CEGUI.h>
class CharacterInfoBox {
public:
  CharacterInfoBox() {}
  ~CharacterInfoBox() {}

  virtual void Initialize();
  virtual void Cleanup();

  virtual void Show();
  virtual void Hide();

  void registerCharacter(Character *character);
  Character *getCharacter() { return m_character; }

protected:
  CEGUI::Window *rootWindow;
  Character *m_character;
  void addText(std::string text);
};

#endif

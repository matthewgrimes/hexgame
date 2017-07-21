#include "CEGUITextLog.h"

CEGUITextLog::CEGUITextLog() {}

CEGUITextLog::~CEGUITextLog() {}
void CEGUITextLog::Init() {

  CEGUI::FontManager::getSingleton().createFromFile("Jura-10.font");
  CEGUI::Window *textLogRoot =
      CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile(
          "TextLog.layout");

  textLog = static_cast<CEGUI::Listbox *>(textLogRoot->getChild("TextLog"));

  CEGUI::System::getSingleton()
      .getDefaultGUIContext()
      .getRootWindow()
      ->addChild(textLog);
}

void CEGUITextLog::addText(const std::string &newText, textLogColor color) {
  CEGUI::Colour ceguiColor =
      CEGUI::Colour(color.R, color.G, color.B, color.Alpha);
  CEGUI::ListboxTextItem *newItem = 0;

  newItem = new CEGUI::ListboxTextItem(newText.c_str());
  newItem->setTextColours(ceguiColor);
  textLog->addItem(newItem);
  textLog->ensureItemIsVisible(textLog->getItemCount());
}

void CEGUITextLog::Hide() { textLog->hide(); }

void CEGUITextLog::Show() { textLog->show(); }

#pragma once

#include "GUISystem.h"
#include "SDLDisplay.h"
#include "TextLog.h"

class ServiceLocator {
public:
  static void Initialize();
  static void Cleanup();
  ~ServiceLocator();

  static GUISystem &getGUISystem() { return *guiSystem; }
  static void setGUISystem(GUISystem *newSystem);

  static TextLog &getTextLog() { return *textLog; }
  static void setTextLog(TextLog *newTextLog);

  static SDLDisplay &getMainDisplay() { return *mainDisplay; }
  static void setMainDisplay(SDLDisplay *newMainDisplay);

private:
  ServiceLocator();

  static GUISystem *guiSystem;
  static NullGUISystem nullGUISystem;

  static TextLog *textLog;
  static NullTextLog nullTextLog;

  static SDLDisplay *mainDisplay;
  static NullDisplay nullDisplay;

  static bool initialized;
};

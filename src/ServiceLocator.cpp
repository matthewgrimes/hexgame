#include "ServiceLocator.h"
#include <assert.h>

NullGUISystem ServiceLocator::nullGUISystem;
GUISystem *ServiceLocator::guiSystem = 0;

NullTextLog ServiceLocator::nullTextLog;
TextLog *ServiceLocator::textLog = &ServiceLocator::nullTextLog;

NullDisplay ServiceLocator::nullDisplay;
SDLDisplay *ServiceLocator::mainDisplay = &ServiceLocator::nullDisplay;

bool ServiceLocator::initialized = false;

void ServiceLocator::setGUISystem(GUISystem *newSystem) {
  if (!newSystem) {
    guiSystem = &nullGUISystem;
    return;
  }
  guiSystem = newSystem;
}

void ServiceLocator::setTextLog(TextLog *newTextLog) {
  if (!newTextLog) {
    textLog = &nullTextLog;
    return;
  }
  textLog = newTextLog;
}

void ServiceLocator::setMainDisplay(SDLDisplay *newMainDisplay) {
  if (!newMainDisplay) {
    mainDisplay = &nullDisplay;
    return;
  }
  mainDisplay = newMainDisplay;
}

ServiceLocator::ServiceLocator() {
  assert(!initialized);
  initialized = true;
}

ServiceLocator::~ServiceLocator() {}

void ServiceLocator::Initialize() {
  guiSystem = &nullGUISystem;
  textLog = &nullTextLog;
  mainDisplay = &nullDisplay;
}

void ServiceLocator::Cleanup() {
  guiSystem->Cleanup();
  delete guiSystem;

  mainDisplay->Cleanup();
  delete mainDisplay;
}

#include "GUISystem.h"

GUISystem::GUISystem() {}

GUISystem::~GUISystem() {}

void NullGUISystem::Initialize() {}

void NullGUISystem::Cleanup() {}

void NullGUISystem::HandleEvent(SDL_Event event) {}

void NullGUISystem::subscribeFunction(std::string windowName,
                                      std::function<void(void)> function) {}

void NullGUISystem::subscribeFunction(std::string parentName,
                                      std::string windowName,
                                      std::function<void(void)> function) {}

void NullGUISystem::addChildFromFile(std::string file) {}

void NullGUISystem::addRootFromFile(std::string file) {}

void NullGUISystem::hideWindow(std::string name) {}

void NullGUISystem::showWindow(std::string name) {}

void NullGUISystem::enableWindow(std::string parentName, std::string name) {}

void NullGUISystem::disableWindow(std::string parentName, std::string name) {}

std::string NullGUISystem::getNameOfWindowContainingMouse() {
  return std::string();
}

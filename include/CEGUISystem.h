#pragma once
#include "GUISystem.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
class CEGUISystem : public GUISystem {
public:
  CEGUISystem();
  ~CEGUISystem();

  virtual void Initialize();
  virtual void Cleanup();
  virtual void HandleEvent(SDL_Event event);

  virtual void subscribeFunction(std::string windowName,
                                 std::function<void(void)> function);
  virtual void subscribeFunction(std::string parentName, std::string windowName,
                                 std::function<void(void)> function);

  virtual void addChildFromFile(std::string file);
  virtual void addRootFromFile(std::string file);

  virtual void hideWindow(std::string name);
  virtual void showWindow(std::string name);

  virtual void enableWindow(std::string parentName, std::string name) override;
  virtual void disableWindow(std::string parentName, std::string name) override;

  virtual void setProperty(const std::string &windowName,
                           const std::string &propertyName,
                           const std::string &value);

  virtual void injectDeltaTime(const float deltaTime);

  virtual void Render();

  virtual std::string getNameOfWindowContainingMouse();

protected:
  void *renderer;
  void set_CEGUI_paths();
  void initCEGUI();

  CEGUI::WindowManager *windowManager;
  CEGUI::Window *rootWindow;
};

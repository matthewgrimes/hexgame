#pragma once
#include <SDL2/SDL.h>
#include <functional>
#include <string>
class GUISystem
{
public:
    GUISystem();
    virtual ~GUISystem();

    virtual void Initialize() = 0;
    virtual void Cleanup() = 0;

    virtual void HandleEvent(SDL_Event event) = 0;

    virtual void subscribeFunction(std::string windowName, std::function<void(void)> function) = 0;
    virtual void subscribeFunction(std::string parentName, std::string windowName, std::function<void(void)> function) = 0;

    virtual void addChildFromFile(std::string file) = 0;
    virtual void addRootFromFile(std::string file) = 0;

    virtual void hideWindow(std::string name) = 0;
    virtual void showWindow(std::string name) = 0;

    virtual void enableWindow(std::string parentName, std::string name) = 0;
    virtual void disableWindow(std::string parentName, std::string name) = 0;

    virtual void setProperty(const std::string& windowName, const std::string& propertyName, const std::string& value) = 0;

    virtual void injectDeltaTime(const float deltaTime) = 0;

    virtual void Render() = 0;

    virtual std::string getNameOfWindowContainingMouse() = 0;
protected:

};

class NullGUISystem : public GUISystem
{
    // Inherited via GUISystem
    virtual void Initialize() override;
    virtual void Cleanup() override;
    virtual void HandleEvent(SDL_Event event) override;
    virtual void subscribeFunction(std::string windowName, std::function<void(void)> function) override;
    virtual void subscribeFunction(std::string parentName, std::string windowName, std::function<void(void)> function) override;
    virtual void addChildFromFile(std::string file) override;
    virtual void addRootFromFile(std::string file) override;

    // Inherited via GUISystem
    virtual void hideWindow(std::string name) override;

    // Inherited via GUISystem
    virtual void showWindow(std::string name) override;
    virtual void setProperty(const std::string& windowName, const std::string& propertyName, const std::string& value) {}
    virtual void injectDeltaTime(const float deltaTime) {}

    virtual void Render() {}

    // Inherited via GUISystem
    virtual void enableWindow(std::string parentName, std::string name) override;

    // Inherited via GUISystem
    virtual void disableWindow(std::string parentName, std::string name) override;

    // Inherited via GUISystem
    virtual std::string getNameOfWindowContainingMouse() override;
};

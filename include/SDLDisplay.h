#pragma once
#include <SDL2/SDL.h>

#include "ShaderManager.h"
#include "ShaderPipeline.h"

struct SDLWindowInfo
{
    char const* name;
    unsigned int width;
    unsigned int height;

    SDLWindowInfo(char const* Name, unsigned int Width, unsigned int Height) : name(Name), width(Width), height(Height) {}
};

class SDLDisplay
{
public:
    SDLDisplay();
    ~SDLDisplay();

    void Initialize(const SDLWindowInfo windowInfo);
    void Cleanup();

    void setupDraw();
    void finishDraw();

    void setupSceneRender();
    void finishSceneRender();

    void setupPickingRender();
    void finishPickingRender();

    void setupGUIRender();
    void finishGUIRender();

    SDL_GLContext openGLContext;
    SDL_Window* getWindow()
    {
        return window;
    }
    SDL_GLContext getOpenGLContext()
    {
        return openGLContext;
    }
    ShaderManager* getShaderManager()
    {
        return &shaderManager;
    }
    ShaderPipeline* getShaderPipeline()
    {
        return &pipeline;
    }
private:
    void initSDL(const SDLWindowInfo windowInfo);
    void initOpenGL();
    void initGlew();
    void initOpenGLMatrices();

    SDL_Window* window;

    ShaderManager shaderManager;
    ShaderPipeline pipeline;
};

class NullDisplay : public SDLDisplay
{
public:
    NullDisplay() {}
    ~NullDisplay() {}

    void Initialize(const SDLWindowInfo windowInfo) {}
    void Cleanup() {}

    void setupDraw() {}
    void finishDraw() {}

    void setupSceneRender() {}
    void setupPickingRender() {}
    void setupGUIRender() {}

    ShaderManager* getShaderManager()	{ return nullptr; }
    ShaderPipeline* getShaderPipeline() { return nullptr; }

};

#include "SDLDisplay.h"

#include <stdio.h>
#include <GL/glew.h>

SDLDisplay::SDLDisplay()
{
    window = nullptr;
    openGLContext = nullptr;
}


SDLDisplay::~SDLDisplay()
{
}

void SDLDisplay::Initialize(const SDLWindowInfo windowInfo)
{
    initSDL(windowInfo);
    initOpenGL();

    shaderManager.CompileShaders("data/shaders/shader","standard");
    shaderManager.CompileShaders("data/shaders/shader_picking", "picking");
    shaderManager.CompileShaders("data/shaders/shader_gui", "gui");
    //shaderManager.CompileShaders("data/shaders/shader_water.vs","data/shaders/shader.gs","data/shaders/shader.fs","water");
    //shaderManager.initFBO(windowInfo.width,windowInfo.height);

    //shaderManager.Activate("standard");
}

void SDLDisplay::Cleanup()
{
    SDL_GL_DeleteContext(openGLContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDLDisplay::setupDraw()
{
    shaderManager.ClearBuffers();
}

/** \brief Finish drawing frame
 *
 *  Call at end of drawing to swap draw and screen buffers.
 */
void SDLDisplay::finishDraw()
{
    SDL_GL_SwapWindow(window);
}

void SDLDisplay::setupSceneRender()
{
    shaderManager.Activate("standard");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    shaderManager.setUniform(pipeline.GetWorldTrans(), "gWorld");
    shaderManager.setUniform(pipeline.getCameraTarget(), "cameraDirection");
    shaderManager.setUniform(0, "gSampler");
    shaderManager.setUniform(1, "gNormal");
}

void SDLDisplay::setupPickingRender()
{
    shaderManager.ActivatePicking();
    shaderManager.resetPickingObjectCount();
    glEnable(GL_DEPTH_TEST);

    shaderManager.setUniform(pipeline.GetWorldTrans(), "gWorld");
    shaderManager.setUniform(pipeline.getCameraTarget(), "cameraDirection");
    shaderManager.setUniform(0, "gSampler");
    shaderManager.setUniform(1, "gNormal");

}

void SDLDisplay::finishPickingRender()
{
    shaderManager.DisablePicking();
}

void SDLDisplay::setupGUIRender()
{
    shaderManager.Activate("gui");

    glDisable(GL_DEPTH_TEST);
}

void SDLDisplay::initSDL(const SDLWindowInfo windowInfo)
{
    int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(windowInfo.name,
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              windowInfo.width,
                              windowInfo.height, flags);
}

void SDLDisplay::initOpenGL()
{
    openGLContext = SDL_GL_CreateContext(window);

    // Use Vsync
    SDL_GL_SetSwapInterval(1);

    initGlew();
    initOpenGLMatrices();

}

void SDLDisplay::initGlew()
{
    GLenum error = glewInit();

    if (error != GL_NO_ERROR)
    {
        printf("Error initializing glew.");
    }
}

void SDLDisplay::initOpenGLMatrices()
{
    GLenum error;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        printf("Error initializing OpenGL Matrices!");
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        printf("Error initializing OpenGL Matrices!");
    }
}

#ifndef CGAMEENGINE_H
#define CGAMEENGINE_H

#include <SDL2/SDL.h>
#include "Timer.h"
#include <vector>
#include <sstream>


#include <GL/glew.h>

//#include "math_utils.h"
class CGameState;
//! Main game class

/**  Creating a single instance
 *  should be enough, it handles
 *  everything else.
 *  In particular, after passing a title,
 *  bit depth, dimensions and fullscreen,
 *  as well as a pointer to the initial game state,
 *  OpenGL and the GUI will be initialized and ready to go.
 */
class CGameEngine
{
public:
    CGameEngine();
    ~CGameEngine();
    //!   Initialize with title, color depth, dimensions, fullscreen, and initial game state.  See CGameState()
    virtual void Init(const char* title, CGameState* initialState);
    virtual void Cleanup();

    /** Move between game states */
    virtual void ChangeState(CGameState* state);
    virtual void PushState(CGameState* state);

    //! Access current game state.
    virtual CGameState* GetCurrentState();
    virtual void PopState();

    //! Handles the various input events
    virtual void HandleEvents();

    virtual void Update();
    virtual void Draw();

    //! Returns whether or not the game engine is running
    bool Running()
    {
        return m_running;
    }
    //! When called, cleans everything up and exits
    void Quit()
    {
        m_running = false;
    }
    //! When called, cleans everything up and exits.


    //! Method for accessing current window
    /*SDL_Window* GetWindow() {
        return sdlWindow;
    }
    //! Method for accessing OpenGL context
    SDL_GLContext GetGLContext() {
        return mainGLContext;
    }*/

private:
    // the stack of states
    std::vector<CGameState*> states;

    bool m_running;
    bool m_fullscreen;


    //SDL_Window *sdlWindow;
    //SDL_GLContext mainGLContext;


    // OpenGL methods
    //bool initGL();

    Timer m_FrameTimer;
    Timer m_MainGameTimer;

};

#endif // CGAMEENGINE_H

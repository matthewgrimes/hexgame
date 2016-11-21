#include "MainMenuState.h"
#include "ServiceLocator.h"
MainMenuState MainMenuState::m_MainMenuState;

void MainMenuState::Init()
{

}

void MainMenuState::Cleanup()
{
}

void MainMenuState::Pause()
{
    ServiceLocator::getGUISystem().hideWindow("MainMenu");
}

void MainMenuState::Resume()
{

    ServiceLocator::getGUISystem().showWindow("MainMenu");
}

void MainMenuState::HandleEvents(CGameEngine* game)
{

    SDL_Event event;
    while ( SDL_PollEvent(&event) )
    {
        // First, pass the event to GUI
        ServiceLocator::getGUISystem().HandleEvent(event);

        // now handle event
        if (event.type == SDL_QUIT)
        {
            game->Quit();
        }
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                game->Quit();
                break;

            case SDLK_h:
                break;

            case SDLK_s:
                break;

            default:
                break;
            }
        }
    }
}

void MainMenuState::Update(CGameEngine* game)
{

}

void MainMenuState::Draw(CGameEngine* game)
{

}

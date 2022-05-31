#include "inputManager.h"
/*
 * ====================================
 * Public methods start here
 * ====================================
 */

void InputManager::clearEventQueue()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {

    }
}

bool InputManager::isGameExiting()
{
    return quit_game;
}

// Polls from keyboard and gets the corresponding action
void InputManager::pollAction(SDL_Event event)
{
    if (event.type == SDL_QUIT)
    {
        quit_game = true;
    }
    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)
    {
        quit_game = true;
    }
}

void InputManager::setExit()
{
    quit_game = true;
}
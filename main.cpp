#include <iostream>
#include <string>

#include <windows.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "board.h"
#include "config.h"
#include "gamestate.h"
#include "init.h"
#include "inputmanager.h"
#include "renderer.h"
#include "texture.h"

int main(int argc, char** argv)
{
    InputManager* inManager = new InputManager;
    GameState gamestate;
    Renderer renderer;
    if (initialize())
    {
        loadMedia();

        gamestate.initializeState();
        gamestate.drawState();
        renderer.updateScreen();

        int countdown = 3;
        Texture countdown_text;
        while (countdown > 0)
        {
            renderer.clearScreen();
            gamestate.drawState();
            countdown_text.loadFromText(std::to_string(countdown), config::default_text_color);
            renderer.renderTexture(&countdown_text, config::logical_window_width / 2, config::logical_window_height / 2);
            renderer.updateScreen();
            SDL_Delay(1000);
            countdown--;
        }

        inManager->clearEventQueue();
        SDL_Event event;
        unsigned long long time_snap1 = SDL_GetTicks();
        while (!inManager->isGameExiting() && !gamestate.isGameOver())
        {
            while (SDL_PollEvent(&event) != 0)
            {
                inManager->pollAction(event);
            }

            unsigned long long time_snap2 = SDL_GetTicks();
            if (time_snap2 - time_snap1 >= config::wait_time)
            {
                gamestate.movePieceDown();
                time_snap1 = SDL_GetTicks();
            }
            renderer.clearScreen();
            gamestate.drawState();
            renderer.updateScreen();
        }

        Texture gameover_text;
        gameover_text.loadFromText("Game Over!", config::default_text_color);
        while (!inManager->isGameExiting())
        {
            while (SDL_PollEvent(&event) != 0)
            {
                inManager->pollAction(event);
            }
            renderer.clearScreen();
            gamestate.drawState();
            renderer.renderTexture(&gameover_text, config::logical_window_width / 2, config::logical_window_height / 2);
            renderer.updateScreen();
        }
    }

    delete inManager;
    std::cerr << "Now exiting!\n";
    close();
    return 0;
}
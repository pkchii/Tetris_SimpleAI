#pragma once
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "texture.h"

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;


class Renderer
{
public:
    void clearScreen();
    void renderTexture(Texture* texture, int x, int y);
    void updateScreen();
};


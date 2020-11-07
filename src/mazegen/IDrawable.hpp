#pragma once

#include <SDL2/SDL_render.h>

class IDrawable
{
  public:
    virtual void Draw(SDL_Renderer *renderer) noexcept = 0;
};

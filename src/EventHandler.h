#pragma once
#include <SDL3/SDL.h>

class EventHandler
{
public:
    virtual ~EventHandler() = default;
    virtual void handleEvent(const SDL_Event& event) = 0;
};


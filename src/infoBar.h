#pragma once
#include <SDL3/SDL.h>

class Texture;

class infoBar
{
private:
    SDL_Renderer* renderer;
    Texture* arrowTexture;   // no es propiedad de infoBar, la libera Game

public:
    infoBar(SDL_Renderer* renderer, Texture* arrowTexture);

    // dibuja la barra con un icono de flecha por cada flecha restante
    void render(int arrowsLeft, int score) const;
};


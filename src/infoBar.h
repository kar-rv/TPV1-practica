#pragma once
#include <SDL3/SDL.h>
#include "GameObject.h"

class Texture;
class Bow;

class infoBar: public GameObject
{
private:
    SDL_Renderer* renderer;
    Texture* arrowTexture;   // no es propiedad de infoBar, la libera Game

public:
    infoBar(SDL_Renderer* renderer, Texture* arrowTexture, Game* game);

    // dibuja la barra con un icono de flecha por cada flecha restante
    void render() const override;
    bool update(float dt) override { return false; }
};


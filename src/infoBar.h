#pragma once
#include <SDL3/SDL.h>
#include "GameObject.h"
#include "GameState.h"

class Texture;
class PlayState;

class infoBar: public GameObject
{
private:
    SDL_Renderer* renderer;
    Texture* arrowTexture;   // no es propiedad de infoBar, la libera Game

public:
    infoBar(SDL_Renderer* renderer, Texture* arrowTexture, PlayState* game);

    // dibuja la barra con un icono de flecha por cada flecha restante
    void render() const override;
    bool update(float dt) override { return false; }
};


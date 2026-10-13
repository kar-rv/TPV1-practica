#include "bow.h"
#include "game.h"
#include "texture.h"
#include <SDL3/SDL_render.h>
bow::bow(Rectangle posDim, Vector2D<> vel, Texture* textura, int numFlechas, bool armado) : 
	posDim(posDim), vel(vel), textura(textura), numFlechas(numFlechas), armado(armado) {
}

bow::~bow() {

}

void
bow::render() const {
	if (textura != nullptr) {
		// Convertimos el Rectangle a SDL_FRect usando to_sdl()
		SDL_FRect destRect = posDim.to_sdl();

		textura->renderFrame(destRect, 0, 0);
	}
}

bool
bow::update(float delta) {
	return false;
}

void
bow::handleEvent(const SDL_Event& event) {
	if (event.key.key == SDLK_DOWN) {
		if (posDim.getTopLeftCorner().getY() + posDim.getHeight() < 600)
			posDim.move(vel);
	}

	else if (event.key.key == SDLK_UP) {
		if(posDim.getTopLeftCorner().getY() >= 1)
		posDim.move(Vector2D<>(-vel.getX(), -vel.getY()));
	}
	
}

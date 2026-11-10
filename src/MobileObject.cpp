#include "MobileObject.h"
#include "game.h"
#include "texture.h"

MobileObject::MobileObject(Rectangle posDim, Texture* textura, Game* game, Vector2D<>vel, int fil) :
	SceneObject(posDim, textura, game), vel(vel), fil(fil) {
}

bool
MobileObject::update(float delta) {

	return false;
}

void
MobileObject::render() const {
	if (textura != nullptr) {
		// convertimos el Rectangle a SDL_FRect usando to_sdl()
		SDL_FRect destRect = posDim.to_sdl();

		textura->renderFrame(destRect, fil, col);
	}
}


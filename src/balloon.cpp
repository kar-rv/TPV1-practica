#include "balloon.h"
#include "texture.h"

balloon::balloon(Rectangle posDim, Vector2D<> vel, Texture* textura, int color) : 
	posDim(posDim), vel(vel), textura(textura), color(color) {

}

balloon::~balloon() {

}

void
balloon::render() const {
	if (textura != nullptr) {
		// Convertimos el Rectangle a SDL_FRect usando to_sdl()
		SDL_FRect destRect = posDim.to_sdl();

		textura->renderFrame(destRect, color, 0);
	}
}

bool
balloon::update(float delta) {
	//si se sale de la pantalla devolvemos true
	if (posDim.getTopLeftCorner().getY() + posDim.getHeight() < 0)
		return true;

	//movemos el glovo hacia ariba
	posDim.move(Vector2D<>(0, vel.getY() * delta));
	return false;
}

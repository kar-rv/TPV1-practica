#include "arrow.h"
#include "texture.h"

arrow::arrow(Rectangle posDim, Vector2D<> vel, Texture* textura) :
	posDim(posDim), vel(vel), textura(textura) {

}

arrow::~arrow() {

}

void
arrow::render() const {
	if (textura != nullptr) {
		// se convierte el Rectangle a SDL_FRect usando to_sdl()
		SDL_FRect destRect = posDim.to_sdl();
		textura->renderFrame(destRect, 0, 0);
	}
}

bool
arrow::update(float delta) {
	posDim.move(Vector2D<>(vel.getX() * delta, vel.getY() * delta));
	// false -> se elimina cuando sale por la derecha
	return posDim.getTopLeftCorner().getX() < Game::WINDOW_WIDTH;
}

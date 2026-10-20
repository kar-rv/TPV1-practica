#include "balloon.h"
#include "texture.h"

using namespace std;

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

		textura->renderFrame(destRect, color, row);
	}
}

bool
balloon::update(float delta) {
	//si se sale de la pantalla devolvemos true
	if (posDim.getTopLeftCorner().getY() + posDim.getHeight() < 0)
		return true;

	//movemos el glovo hacia ariba

	if (dead) {
		SDL_FRect destRect = posDim.to_sdl();
		animationTimer += delta;
		if (animationTimer > 0.05f) {
			row++;
			animationTimer = 0.0f;
		}
		if (row > 6) {
			return true;
		}
	}
	else
		posDim.move(Vector2D<>(0, vel.getY() * delta));

	return false;
}

bool
balloon::hit(const Rectangle& obj) {
	if (!dead && posDim.hasIntersection(obj)) {
		row = 1;
		dead = true;
		return true;
	}
	return false;
}

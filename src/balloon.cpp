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

	if (dead) {
		SDL_FRect destRect = posDim.to_sdl();
		cout << "empieza animacion" << endl;
		animationTimer += delta;
		if (animationTimer > 1.0f) {
			textura->renderFrame(destRect, row, color);
			cout << "bucleee" << endl;
			row++;
			animationTimer = 0.0f;
		}
		return true;
	}
	return false;
}

bool
balloon::hit(const Rectangle& obj) {
	if (posDim.hasIntersection(obj)) {
		dead = true;
		return true;
	}
	return false;
}

bool
balloon::animate(float delta) {
	SDL_FRect destRect = posDim.to_sdl();
	cout << "empieza animacion" << endl;
	static float animationTimer = 0.0f;

	int row = 1;
	while (row < 6) {
		animationTimer += delta;
		if (animationTimer > 10.0f) {
			textura->renderFrame(destRect, row, color);
			cout << "bucleee" << endl;
			row++;
			animationTimer = 0.0f;
		}
	}
	return true;
}

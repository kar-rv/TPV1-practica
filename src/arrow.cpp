#include "arrow.h"
#include "texture.h"
#include "game.h"

using namespace std;

arrow::arrow(Rectangle posDim, Texture* textura, Game* game, Vector2D<>vel) :
	MobileObject(posDim, textura, game, vel) {

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
	Rectangle rect(Point2D<>(posDim.getTopLeftCorner().getX() + 60, posDim.getTopLeftCorner().getY()), 5, 5);
	game->checkCollision(rect);
	return posDim.getTopLeftCorner().getX() < Game::WINDOW_WIDTH;
}

#include "Butterfly.h"
#include "game.h"

Butterfly::Butterfly(Rectangle posDim, Vector2D<>vel, Texture* textura, Game* game) :
	MobileObject(posDim, textura, game, vel){

}

Butterfly::~Butterfly() {

}

bool
Butterfly::update(float delta) {
	if (posDim.getTopLeftCorner().getX() > Game::WINDOW_WIDTH - posDim.getWidth() ||
		posDim.getTopLeftCorner().getX() < 0) {
		vel.invertX();
	}
	if (posDim.getTopLeftCorner().getY() > Game::WINDOW_HEIGHT - posDim.getHeight() ||
		posDim.getTopLeftCorner().getY() < 0) {
		vel.invertY();
	}
	posDim.move(Vector2D<>(vel.getX() * delta, vel.getY() * delta));
	return false;
}

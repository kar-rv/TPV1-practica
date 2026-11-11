#include "arrow.h"
#include "texture.h"
#include "game.h"

using namespace std;

arrow::arrow(Rectangle posDim, Texture* textura, Game* game, Vector2D<>vel) :
	MobileObject(posDim, textura, game, vel) {

}

arrow::~arrow() {

}

bool
arrow::update(float delta) {
	posDim.move(Vector2D<>(vel.getX() * delta, vel.getY() * delta));
	// false -> se elimina cuando sale por la derecha
	Rectangle rect(Point2D<>(posDim.getTopLeftCorner().getX() + 60, posDim.getTopLeftCorner().getY()), 5, 5);
	game->checkCollision(rect);
	return posDim.getTopLeftCorner().getX() > Game::WINDOW_WIDTH;
}

void
arrow::saveValues(std::ostream& save) {
	// por orden se guarda: posicion(x, y), dimensiones(w, h), velocidad(u, v)
	save << "arrow " << posDim.getTopLeftCorner().getX() << " " << posDim.getTopLeftCorner().getY() << " "
		<< posDim.getWidth() << " " << posDim.getHeight() << " " << vel.getX() << " " << vel.getY() << " " << endl;
}

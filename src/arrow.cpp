#include "arrow.h"
#include "texture.h"
#include "PlayState.h"
#include "GameError.h"
#include "game.h"

using namespace std;

arrow::arrow(Rectangle posDim, Texture* textura, PlayState* game, Vector2D<>vel) :
	MobileObject(posDim, textura, game, vel) {
	game->arrowCreated();
}

arrow::~arrow() {
	game->arrowDestroyed();
}

bool
arrow::update(float delta) {
	posDim.move(Vector2D<>(vel.getX() * delta, vel.getY() * delta));
	Rectangle rect(Point2D<>(posDim.getTopLeftCorner().getX() + 60, posDim.getTopLeftCorner().getY()), 5, 5);
	// globos explotados por la flecha
	const int poppedBefore = game->getBalloonsPopped();
	game->checkCollision(rect);
	const int currentPopped = game->getBalloonsPopped() - poppedBefore;
	// si se ha explotado mas de un globo se añade el bonus
	for (int i = 0; i < currentPopped; ++i) {
		++combo;
		if (combo > 1)
			game->changeScore(Game::COMBO_BONUS * combo);
	}
	// false -> se elimina cuando sale por la derecha
	return posDim.getTopLeftCorner().getX() > Game::WINDOW_WIDTH;
}

void
arrow::saveValues(std::ostream& save) {
	// por orden se guarda: posicion(x, y), dimensiones(w, h), velocidad(u, v)
	save << "arrow " << posDim.getTopLeftCorner().getX() << " " << posDim.getTopLeftCorner().getY() << " "
		<< posDim.getWidth() << " " << posDim.getHeight() << " " << vel.getX() << " " << vel.getY() << " "
		<< combo << endl;
}

void
arrow::loadValues(std::istream& load) {
	MobileObject::loadValues(load);
	if (!(load >> combo) || combo < 0) {
		throw GameError("Error: datos de flecha incorrectos");
	}
}

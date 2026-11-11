#include "balloon.h"
#include "texture.h"
#include "game.h"

using namespace std;

balloon::balloon(Rectangle posDim, Vector2D<> vel, Texture* textura, int fil, Game* game) : 
	MobileObject(posDim,  textura, game, vel, fil) {

}

balloon::~balloon() {

}

bool
balloon::update(float delta) {
	//si se sale de la pantalla devolvemos true
	if (posDim.getTopLeftCorner().getY() + posDim.getHeight() < 0)
		return true;

	if (dead) {
		SDL_FRect destRect = posDim.to_sdl();
		animationTimer += delta;
		if (animationTimer > 0.05f) {
			col++;
			animationTimer = 0.0f;
		}
		if (col > 6) {
			return true;
		}
	}
	else
		//movemos el glovo hacia ariba
		posDim.move(Vector2D<>(0, vel.getY() * delta));

	return false;
}

bool
balloon::hit(const Rectangle& obj) {
	if (!dead && posDim.hasIntersection(obj)) {
		col = 1;
		dead = true;
		//actualizamos el score del game
		game->changeScore(game->getBalPoints());
		return true;
	}
	return false;
}

void
balloon::saveValues(std::ostream& save) {
	char n = 'n';
	if (dead) n = 'e';
	// por orden se guarda: posicion(x, y), dimensiones(w, h), velocidad(u, v), explotado(e -> si / n -> no), color
	save << "balloon " << posDim.getTopLeftCorner().getX() << " " << posDim.getTopLeftCorner().getY() << " "
		<< posDim.getWidth() << " " << posDim.getHeight() << " " << vel.getX() << " " << vel.getY() << " "
		<< n << " " << fil << endl;
}

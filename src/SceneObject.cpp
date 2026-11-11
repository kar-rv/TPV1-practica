#include "SceneObject.h"
#include "game.h"
#include "texture.h"

SceneObject::SceneObject(Rectangle posDim, Texture* textura, Game* game) :
	GameObject(game), posDim(posDim), textura(textura) {

}
bool SceneObject::hit(const Rectangle& other){
	// Comprobación básica de intersección de rectángulos usando el método de Rectangle
	return posDim.hasIntersection(other);
}
void
SceneObject::render() const {
	if (textura != nullptr) {
		// se convierte el Rectangle a SDL_FRect usando to_sdl()
		SDL_FRect destRect = posDim.to_sdl();
		textura->renderFrame(destRect, 0, 0);
	}
}

void
SceneObject::saveValues(std::ostream& save) {
	return;
}

void
SceneObject::loadValues(std::istream& load) {
	float x, y, w, h;
	if (!(load >> x >> y >> w >> h)) {
		throw std::string("Error: posicion y/o dimensiones incorrectas");
	}
		
	posDim = Rectangle(Point2D<>(x, y), w, h);
}


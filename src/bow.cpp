#include "bow.h"
#include "game.h"
#include "texture.h"
#include <SDL3/SDL_render.h>
#include <iostream>

using namespace std;

bow::bow(Rectangle posDim, Vector2D<> vel, Texture* textura, int numFlechas, bool armado, Game* game) : 
	SceneObject(posDim, textura, game), vel(vel), numFlechas(numFlechas), armado(armado) {
}

bow::~bow() {

}

void
bow::render() const {
	if (textura != nullptr) {
		// se convierte el Rectangle a SDL_FRect usando to_sdl()
		SDL_FRect destRect = posDim.to_sdl();
		if (armado) textura->renderFrame(destRect, 0, 0);
		else textura->renderFrame(destRect, 0, 1);	
	}
}

bool
bow::update(float delta) {
	return false;
}

void
bow::handleEvent(const SDL_Event& event) {
	// si no es una pulsación de tecla, no se hace nada
	if (event.type != SDL_EVENT_KEY_DOWN)
		return;
	cout << "tecla pulsada" << endl;

	if (event.key.key == SDLK_DOWN) {
		if (posDim.getTopLeftCorner().getY() + posDim.getHeight() < 600)
			posDim.move(vel);
	}

	else if (event.key.key == SDLK_UP) {
		if(posDim.getTopLeftCorner().getY() >= 1)
			posDim.move(Vector2D<>(-vel.getX(), -vel.getY()));
	}

	else if (event.key.key == SDLK_LEFT) {
		if (numFlechas > 0) {
			armado = true;  // cargar flecha
		}
		cout << "armado" << endl;
	}

	else if (event.key.key == SDLK_RIGHT) {
		cout << "armado = " << armado << " flechas = " << numFlechas << endl;
		if (armado && numFlechas > 0) {
			// Punto de salida: borde derecho del arco, a media altura
			Point2D<> origin(posDim.getTopLeftCorner().getX() + posDim.getWidth() / 2,
				posDim.getTopLeftCorner().getY() + posDim.getHeight() / 2);
			game->generateArrow(origin);
			armado = false;
			--numFlechas;
		}
	}
	
}

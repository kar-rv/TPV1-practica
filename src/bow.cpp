#include "bow.h"
#include "texture.h"
#include <SDL3/SDL_render.h>
bow::bow(Rectangle posDim, Vector2D<> vel, Texture* textura, int numFlechas, bool armado) : 
	posDim(posDim), vel(vel), textura(textura), numFlechas(numFlechas), armado(armado) {
}

bow::~bow() {

}

void
bow::render() const {
	if (textura != nullptr) {
		// Convertimos el Rectangle a SDL_FRect usando to_sdl()
		SDL_FRect destRect = posDim.to_sdl();

		// Como tu textura del arco tiene 1 fila y 2 columnas (según textureList), 
		// puedes renderizar la columna 0 (por ejemplo, el arco sin armar o normal)
		textura->renderFrame(destRect, 0, 0);
	}
}

bool
bow::update(float delta) {
	return false;
}

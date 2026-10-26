#include "infoBar.h"
#include "texture.h"
#include "game.h"
#include <string>

using namespace std;

infoBar::infoBar(SDL_Renderer* renderer, Texture* arrowTexture)
	: renderer(renderer), arrowTexture(arrowTexture) {
}

void
infoBar::render(int arrowsLeft, int score) const {
	const float margin = 8;
	const float arrowWidth = 30;
	const float spacing = 6;  // espacio entre flecha y flecha
	// se mantiene la proporción de la imagen de la flecha
	const float arrowHeight = arrowWidth / arrowTexture->getAspectRatio();

	// fondo semitransparente en la parte superior de la ventana
	SDL_FRect fondo{ 0, 0, float(Game::WINDOW_WIDTH), arrowHeight + 2 * margin };
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
	SDL_RenderFillRect(renderer, &fondo);  // se rellena el fondo

	// un icono por cada flecha restante
	for (int i = 0; i < arrowsLeft; ++i) {
		SDL_FRect srcRect{ margin + i * (arrowWidth + spacing), margin, arrowWidth, arrowHeight };
		arrowTexture->render(srcRect);
	}

	// puntuación
	const float scale = 2.0f;  // como el texto se ve muy pequeño por defecto pues se multiplica por dos su tamaño
	string text = "SCORE: " + to_string(score);
	float textWidth = text.size() * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;  // esto calcula el tamaño de la fuente en pixeles

	SDL_SetRenderScale(renderer, scale, scale);  // se amplia la escala
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDebugText(renderer,
		Game::WINDOW_WIDTH / scale - textWidth - margin / scale,
		// esto es para centrar el texto en vertical (centro - altura / 2)
		fondo.h / scale / 2 - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE / 2.0f,
		text.c_str());  // convierte un string en un const char* (que es lo que pide el SDL_RenderDebugText)
	SDL_SetRenderScale(renderer, 1.0f, 1.0f);  // se vuelve al valor por defecto de la escala
}

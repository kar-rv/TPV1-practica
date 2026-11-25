#include "infoBar.h"
#include "texture.h"
#include "PlayState.h"
#include "bow.h"
#include <string>
#include "game.h"

using namespace std;

infoBar::infoBar(SDL_Renderer* renderer, Texture* arrowTexture, PlayState* game)
	: GameObject(game), renderer(renderer), arrowTexture(arrowTexture) {
}

void
infoBar::render() const {
	if (game == nullptr || game->getBow() == nullptr) {
		return;
	}
	int arrowsLeft = game->getBow()->getNumFlechas();
	int score = game->getScore();

	const float margin = 15;
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
	Texture* digitTexture = game->getGame()->getTexture(Game::DIGITS);
	int points = score;
	float x = Game::WINDOW_WIDTH - 30.0f;
	if (score == 0) {
		Rectangle digitRectangle(Point2D<>(x, 7.0f), 20.0f, 20.0f);
		SDL_FRect digitRect = digitRectangle.to_sdl();
		digitTexture->renderFrame(digitRect, 0, 0);
	}
	while (points != 0) {
		int digit = points % 10;
		points = points / 10;
		Rectangle digitRectangle(Point2D<>(x, 10.0f), 20.0f, 20.0f);
		x -= 22;
		SDL_FRect digitRect = digitRectangle.to_sdl();
		digitTexture->renderFrame(digitRect, 0, digit);
	}
}

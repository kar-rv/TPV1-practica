#include "MainMenuState.h"
#include "game.h"
#include "PlayState.h"
#include "texture.h"

MainMenuState::MainMenuState(Game* game)
	: GameState(game),
	nuevaPartida(Point2D<>(Game::WINDOW_WIDTH / 2.0f - 236.0f / 2.0f, 220.0f), 236.0f, 48.0f),
	cargarPartida(Point2D<>(Game::WINDOW_WIDTH / 2.0f - 250.0f / 2.0f, 320.0f), 250.0f, 48.0f),
	salir(Point2D<>(Game::WINDOW_WIDTH / 2.0f - 73.0f / 2.0f, 420.0f), 73.0f, 48.0f)
{

}

bool MainMenuState::isPointInside(float x, float y, const Rectangle& rect) const
{
	float rx = rect.getTopLeftCorner().getX();
	float ry = rect.getTopLeftCorner().getY();
	float rw = rect.getWidth();
	float rh = rect.getHeight();
	return (x >= rx && x <= rx + rw && y >= ry && y <= ry + rh);
}

bool MainMenuState::update(float delta)
{
	return false;
}

void MainMenuState::render() const
{
	SDL_Renderer* renderer = game->getRenderer();

	SDL_SetRenderDrawColor(renderer, 25, 30, 45, 255);
	SDL_RenderClear(renderer);

	Texture* bgTexture = game->getTexture(Game::MENUBACKGROUND);
	bgTexture->render();

	// Dibujo de ejemplo de los botones con SDL_FRect
	SDL_FRect nuevaPartidaRect = nuevaPartida.to_sdl();
	Texture* nuevaPartidaTexture = game->getTexture(Game::NUEVAPARTIDA);
	nuevaPartidaTexture->render(nuevaPartidaRect);

	SDL_FRect cargarPartidaRect = cargarPartida.to_sdl();
	Texture* cargarPartidaTexture = game->getTexture(Game::CARGAPARTIDA);
	cargarPartidaTexture->render(cargarPartidaRect);

	SDL_FRect salirRect = salir.to_sdl();
	Texture* salirTexture = game->getTexture(Game::SALIR);
	salirTexture->render(salirRect);


	SDL_RenderPresent(renderer);
}

void MainMenuState::handleEvent(const SDL_Event& event)
{
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
		float mx = event.button.x;
		float my = event.button.y;

		if (isPointInside(mx, my, nuevaPartida)) {
			game->replaceState(new PlayState(game, false));
		}
		else if (isPointInside(mx, my, cargarPartida)) {
			game->replaceState(new PlayState(game, true));
		}
		else if (isPointInside(mx, my, salir)) {
			game->exitGame();
			game->popState();
		}
	}
}

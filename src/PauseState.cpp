#include "PauseState.h"
#include "game.h"
#include "PlayState.h"
#include "MainMenuState.h"
#include "texture.h"
#include "GameError.h"

PauseState::PauseState(Game* game, PlayState* playState)
	: GameState(game), playState(playState),
	continuar(Point2D<>(Game::WINDOW_WIDTH / 2.0f - 166.0f / 2.0f, 90.0f), 166.0f, 48.0f),
	guardarPartida(Point2D<>(Game::WINDOW_WIDTH / 2.0f - 273.0f / 2.0f, 215.0f), 273.0f, 48.0f),
	cargarPartida(Point2D<>(Game::WINDOW_WIDTH / 2.0f - 250.0f / 2.0f, 290.0f), 250.0f, 48.0f),
	volverAlMenu(Point2D<>(Game::WINDOW_WIDTH / 2.0f - 248.0f / 2.0f, 415.0f), 248.0f, 48.0f),
	salir(Point2D<>(Game::WINDOW_WIDTH / 2.0f - 73.0f / 2.0f, 490.0f), 73.0f, 48.0f)
{

}

bool PauseState::isPointInside(float x, float y, const Rectangle& rect) const
{
	float rx = rect.getTopLeftCorner().getX();
	float ry = rect.getTopLeftCorner().getY();
	float rw = rect.getWidth();
	float rh = rect.getHeight();
	return (x >= rx && x <= rx + rw && y >= ry && y <= ry + rh);
}

bool PauseState::update(float delta)
{
	return false;
}

void PauseState::render() const
{
	SDL_Renderer* renderer = game->getRenderer();
	if (renderer == nullptr) return;

	if (playState != nullptr) {
		playState->render();
	}

	SDL_SetRenderScale(renderer, 1.0f, 1.0f);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);

	SDL_FRect fondo{ 0.0f, 0.0f, float(Game::WINDOW_WIDTH), float(Game::WINDOW_HEIGHT) };
	SDL_RenderFillRect(renderer, &fondo);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_FRect continuarRect = continuar.to_sdl();
	Texture* continuarTexture = game->getTexture(Game::CONTINUAR);
	continuarTexture->render(continuarRect);

	SDL_FRect guardarPartidaRect = guardarPartida.to_sdl();
	Texture* guardarPartidaTexture = game->getTexture(Game::GUARDAPARTIDA);
	guardarPartidaTexture->render(guardarPartidaRect);

	SDL_FRect cargarPartidaRect = cargarPartida.to_sdl();
	Texture* cargarPartidaTexture = game->getTexture(Game::CARGAPARTIDA);
	cargarPartidaTexture->render(cargarPartidaRect);

	SDL_FRect volverAlMenuRect = volverAlMenu.to_sdl();
	Texture* volverAlMenuTexture = game->getTexture(Game::VOLVERALMENU);
	volverAlMenuTexture->render(volverAlMenuRect);

	SDL_FRect salirRect = salir.to_sdl();
	Texture* salirTexture = game->getTexture(Game::SALIR);
	salirTexture->render(salirRect);
	

	SDL_RenderPresent(renderer);
}

void PauseState::handleEvent(const SDL_Event& event)
{
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
		float mx = event.button.x;
		float my = event.button.y;
		if (isPointInside(mx, my, continuar)) {
			game->popState();
			playState->pause();
		}
		else if (isPointInside(mx, my, guardarPartida)) {
			try {
				playState->saveLevel("../assets/saves/saved.txt");
				playState->pause();
			}
			catch (const GameError& e) {
				std::cerr << e.what() << std::endl;
			}
			game->popState();
		}
		else if (isPointInside(mx, my, cargarPartida)) {
			try {
				playState->loadSavedLevel("../assets/saves/saved.txt");
				playState->pause(); // reanuda al volver a la partida cargada
			}
			catch (const GameError& e) {
				std::cerr << e.what() << std::endl;
			}
			game->popState();
		}
		else if (isPointInside(mx, my, volverAlMenu)) {
			playState->pause();
			game->replaceState(new MainMenuState(game));
		}
		else if (isPointInside(mx, my, salir)) {
			game->exitGame();
			game->popState();
		}
	}
}


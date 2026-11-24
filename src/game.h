#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>

#include "gameStateMachine.h"

class Texture;

class Game : public GameStateMachine
{
public:
	static constexpr int FRAME_RATE = 30;
	static constexpr int FRAME_PERIOD = 1000 / FRAME_RATE;
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;
	static constexpr int COMBO_BONUS = 5;

	enum TextureName
	{
		BOW = 0,
		ARROW,
		BALLOON,
		BUTTERFLY,
		DIGITS,
		REWARDS,
		MENUBACKGROUND,
		CARGAPARTIDA,
		CONTINUAR,
		GUARDAPARTIDA,
		NIVEL,
		NUEVAPARTIDA,
		PUNTUACION,
		SALIR,
		VOLVERALMENU,
		NUM_TEXTURES
	};

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	std::array<Texture*, NUM_TEXTURES> textures{};

	Uint64 lastTime = 0;
	bool exit = false;

	void handleEvents();

public:
	Game();
	~Game();

	Texture* getTexture(TextureName name) const { return textures[name]; }
	void run();

	SDL_Window* getWindow() const { return window; }
	SDL_Renderer* getRenderer() const { return renderer; }
};

#endif // GAME_H

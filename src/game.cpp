#include "game.h"
#include <iostream>
#include <string>

#include <SDL3_image/SDL_image.h>

#include "GameError.h"
#include "texture.h"
#include "PlayState.h"
#include "MainMenuState.h"

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 2.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";

// Estructura para especificar las texturas que hay que cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
	
	TextureSpec{"bow.png", 1, 2},
	TextureSpec{"arrow.png"},
	TextureSpec{"balloons.png", 7, 6},
	TextureSpec{"butterfly.png", 4, 10},
	TextureSpec{"digits.png", 1, 10},
	TextureSpec{"rewards.png", 10, 8},
	TextureSpec{"menuBackground.png"},
	TextureSpec{"cargarPartida.png"},
	TextureSpec{"continuar.png"},
	TextureSpec{"guardarPartida.png"},
	TextureSpec{"nivel.png"},
	TextureSpec{"nuevaPartida.png"},
	TextureSpec{"puntuacion.png"},
	TextureSpec{"salir.png"},
	TextureSpec{"volverAlMenu.png"}
};

Game::Game()
{
	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO)) throw SDLError("SDL_Init");

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
	                                 WINDOW_WIDTH, WINDOW_HEIGHT,
	                                 0, &window, &renderer))
		throw SDLError("SDL_CreateWindowAndRenderer");
	size_t i = 0;
	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;

		for (i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
	}
	catch (...) {
		// liberar la memoria reservada
		for (size_t j = 0; j < i; j++) {
			delete textures[j];
			textures[j] = nullptr;
		}
		throw;
	}
	
	pushState(new MainMenuState(this));
	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

Game::~Game()
{
	for (Texture* texture : textures) {
		delete texture;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void
Game::run()
{
	while (!exit) {
		// calcular el Delta Time en segundos
		Uint64 currentTime = SDL_GetTicks();
		float delta = (currentTime - lastTime) / 1000.0f; // Pasamos milisegundos a segundos
		lastTime = currentTime;

		// se limita el delta por si la ventana se arrastra o se congela
		if (delta > 0.1f) delta = 0.1f;

		render();
		update(delta);
		handleEvents();

		// se espera lo que falte hasta completar el periodo del frame
		Uint64 elapsed = SDL_GetTicks() - currentTime;
		if (elapsed < FRAME_PERIOD)
			SDL_Delay(FRAME_PERIOD - elapsed);
	}
}
void Game::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			exit = true;
		}
		GameStateMachine::handleEvent(event);
	}
}

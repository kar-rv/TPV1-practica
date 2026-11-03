#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include <list>
#include "rectangle.h"
#include "arrow.h"
#include "SceneObject.h"
#include <random>

// Declaraciones anticipadas
class Texture;
class arrow;
class balloon;
class bow;
class infoBar;

/**
 * Clase principal del juego.
 */
class Game
{
public:
	// Número de actualizaciones por segundo
	static constexpr int FRAME_RATE = 30;
	// Periodo entre actualizaciones en milisegundos
	static constexpr int FRAME_PERIOD = 1000 / FRAME_RATE;
	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;

	enum TextureName
	{
		BOW = 0,
		ARROW,
		BALLOON,
		BUTTERFLY,
		BACKGROUND,
		NUM_TEXTURES
	};

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;

	void render() const;
	void update(float delta);
	void handleEvents();
	void generateBalloon();
	bool loadLevel(std::string route);
	void generarButterflies();

	bool exit;
	Uint64 lastTime;

	// Elemento del juego
	// TODO: añadir atributos para los objetos del juego
	std::list<SceneObject*> objects;
	bow* bowPtr;
	infoBar* infoBarPtr;
	std::mt19937_64 random;

	int numArrows = 10;
	int numButterflies = 10;
	float balloonDelay = 2.0f;
	int score = 0;

public:
	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();

	// Comprueba si hay algún objeto colocado en ese rectángulo
	bool checkCollision(const Rectangle& rect);
	void generateArrow(const Point2D<>& pos);

	int getScore() const {
		return score;
	}

	bow* getBow() const {
		return bowPtr;
	}
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}
#endif // GAME_H

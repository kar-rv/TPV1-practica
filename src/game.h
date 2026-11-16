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
#include <string>

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
	// combo globos
	static constexpr int COMBO_BONUS = 5;

	enum TextureName
	{
		BOW = 0,
		ARROW,
		BALLOON,
		BUTTERFLY,
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
	void generarButterflies();
	void checkGoal();
	std::string levelPath(int level) const;
	bool levelExists(int level) const;
	void generateNewLevel();
	void end();

	// metodos de carga / guardado de niveles
	void saveLevel(const std::string route);
	void loadSavedLevel(const std::string route);
	void loadLevel(const std::string& route);
	void parseLevelLine(const std::string& route, int lineNum, const std::string& word, std::istream& iss);
	void readSavedHeader(std::istream& load, const std::string& route, int& lvl, int& pts);
	void readSavedObjects(std::istream& load, const std::string& route, int& lineNum, std::list<SceneObject*>& objs, bow*& newBow);
	SceneObject* createSavedObject(const std::string& word);
	void applyLoadedGame(std::list<SceneObject*>& objs, bow* newBow, int lvl, int pts);

	bool exit;
	Uint64 lastTime;

	// Elemento del juego
	// TODO: añadir atributos para los objetos del juego
	std::list<SceneObject*> objects;
	bow* bowPtr = nullptr;
	infoBar* infoBarPtr = nullptr;
	std::mt19937_64 random;

	int r = 0, g = 0, b = 0;
	int numArrows = 0;
	int goal = 0;
	int numButterflies = 0;
	float balloonDelay = 2.0f;
	int balPoints = 0;
	float balSpeed = 0;
	int score = 0;
	int lvlActual = 0;
	int butPoints = -20;
	int balloonsPopped = 0;

	bool juegoTerminado = false;

public:
	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();
	void balloonPopped();  // combo de globos (esta en public porque lo usa balloon.cpp

	// Comprueba si hay algún objeto colocado en ese rectángulo
	bool checkCollision(const Rectangle& rect);
	void generateArrow(const Point2D<>& pos);

	int getScore() const {
		return score;
	}
	void changeScore(int points) {
		score += points;
	}

	int getBalPoints() const { 
		return balPoints;
	}
	int getButPoints() const { 
		return butPoints;
	}
	int getBalloonsPopped() const {
		return balloonsPopped;
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

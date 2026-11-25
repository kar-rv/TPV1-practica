#pragma once

#include <list>
#include <random>
#include <string>

#include "GameState.h"
#include "SceneObject.h"
#include "rectangle.h"

class arrow;
class balloon;
class bow;
class infoBar;

class PlayState : public GameState
{
private:
	std::list<SceneObject*> objects;
	bow* bowPtr = nullptr;
	infoBar* infoBarPtr = nullptr;

	int r = 0, g = 0, b = 0;
	int numArrows = 0;
	int goal = 0;
	int numButterflies = 0;
	float balloonDelay = 2.0f;
	int balPoints = 0;
	float balSpeed = 0.0f;
	int score = 0;
	int lvlActual = 0;
	int butPoints = -20;
	int balloonsPopped = 0;
	int arrowsInFlight = 0;

	bool juegoTerminado = false;
	bool juegoPerdido = false;
	bool juegoPausado = false;

	float timeSinceLastBalloonSpawn = 0.0f;

	std::mt19937_64 random;

	void generateBalloon();
	void generarButterflies();
	void checkGoal();
	std::string levelPath(int level) const;
	bool levelExists(int level) const;
	void generateNewLevel();
	void checkDefeat();
	void end();

	// Métodos de carga / guardado de niveles
	void loadLevel(const std::string& route);
	void parseLevelLine(const std::string& route, int lineNum, const std::string& word, std::istream& iss);
	void readSavedHeader(std::istream& load, const std::string& route, int& lvl, int& pts);
	void readSavedObjects(std::istream& load, const std::string& route, int& lineNum, std::list<SceneObject*>& objs, bow*& newBow);
	SceneObject* createSavedObject(const std::string& word);

public:
	PlayState(Game* game, bool cargarGuardada = false);
	~PlayState() override;

	bool update(float delta) override;
	void render() const override;
	void handleEvent(const SDL_Event& event) override;

	//cargar y guardar el nivel (publivos para acceder desde pauseState
	void saveLevel(const std::string& route);
	void loadSavedLevel(const std::string& route);

	void balloonPopped() { ++balloonsPopped; }
	void arrowCreated() { ++arrowsInFlight; }
	void arrowDestroyed() { --arrowsInFlight; }

	bool checkCollision(const Rectangle& rect);
	void generateArrow(const Point2D<>& pos);

	int getScore() const { return score; }
	void changeScore(int points) { score += points; }

	int getBalPoints() const { return balPoints; }
	int getButPoints() const { return butPoints; }
	int getBalloonsPopped() const { return balloonsPopped; }

	void pause() { juegoPausado = !juegoPausado; }
	bow* getBow() const { return bowPtr; }
	Game* getGame() const { return game; }
};

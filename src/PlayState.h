#pragma once
#include <list>
#include "GameState.h"
#include "SceneObject.h"
#include <vector>
#include <list>
#include "rectangle.h"
#include "arrow.h"
#include "SceneObject.h"
#include <random>
#include <string>

struct Rectangle;
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
	float balSpeed = 0;
	int score = 0;
	int lvlActual = 0;
	int butPoints = -20;
	int balloonsPopped = 0;
	int arrowsInFlight = 0;

	bool juegoTerminado = false;
	bool juegoPerdido = false;

	void handleEvents();
	void generateBalloon();
	void generarButterflies();
	void checkGoal();
	std::string levelPath(int level) const;
	bool levelExists(int level) const;
	void generateNewLevel();
	void checkDefeat();
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
	std::mt19937_64 random;

public:
    PlayState(Game* g);
    ~PlayState() override;

    bool update(float delta) override;
    void render() const override;

    //cosas que antes estaban en game

	void balloonPopped() { ++balloonsPopped; }  // combo de globos
	// esto es para que cuando no quede ninguna flecha ni en la pantalla ni en el arco se acabe el juego
	void arrowCreated() { ++arrowsInFlight; }
	void arrowDestroyed() { --arrowsInFlight; }

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


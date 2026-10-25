#pragma once
#include <iostream>
#include "rectangle.h"
#include "vector2D.h"
#include "game.h"
#include <SDL3/SDL_events.h>

class Texture;
class Game;

class bow
{
private:
	Rectangle posDim;
	Vector2D<> vel;
	int numFlechas;
	bool armado;

	Texture* textura;
	Game* game;  // puntero a game para acceder a la función que genera las flechas

public:
	bow(Rectangle posDim, Vector2D<> vel, Texture* textura, int numFlechas, bool armado = false, Game* game = nullptr);
	~bow();
	void render() const;
	bool update(float delta);
	void handleEvent(const SDL_Event& event);

	int getNumFlechas() const {
		return numFlechas;
	}
};


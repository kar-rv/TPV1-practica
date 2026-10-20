#pragma once
#include "game.h"
#include "rectangle.h"
#include "vector2D.h"

class Texture;
class Game;

class arrow
{
private:
	Game* game;
	Rectangle posDim;
	Vector2D<> vel;

	Texture* textura;

public:
	arrow(Rectangle posDim, Vector2D<>vel, Texture* textura, Game* game = nullptr);
	~arrow();
	void render() const;
	bool update(float delta);
};


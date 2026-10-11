#pragma once
#include "game.h"
#include "rectangle.h"
#include "vector2D.h"

class Texture;

class arrow
{
private:
	Game* game;
	Rectangle posDim;
	Vector2D<> vel;

	Texture* textura;

public:
	void render() const;
	bool update(float delta);
};


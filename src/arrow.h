#pragma once
#include "game.h"
#include "rectangle.h"
#include "vector2D.h"
#include "MobileObject.h"

class Game;
class Texture;

class arrow : public MobileObject
{
public:
	arrow(Rectangle posDim, Texture* textura, Game* game, Vector2D<>vel);
	~arrow();
	void render() const;
	bool update(float delta);
};


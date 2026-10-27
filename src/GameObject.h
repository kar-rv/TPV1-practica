#pragma once
#include "game.h"

class Game;

class GameObject
{
public:
	GameObject(Game* game);
	virtual void render() const = 0;
	virtual bool update(float dt) = 0;

protected:
	Game* game;
};


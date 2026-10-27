#pragma once
#include "SceneObject.h"

class Texture;
class Game;

class MobileObject : public SceneObject
{
protected:
	Vector2D<> vel;

public:
	MobileObject(Rectangle posDim, Texture* textura, Game* game, Vector2D<>vel);
	bool update(float dt) override;
};


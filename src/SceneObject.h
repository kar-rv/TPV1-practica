#pragma once
#include "GameObject.h"
#include "rectangle.h"

class Texture;
class Game;

class SceneObject: public GameObject
{
protected:
	Rectangle posDim;
	Texture* textura;

public:
	SceneObject(Rectangle posDim, Texture* textura, Game* game);
	virtual bool hit(const Rectangle&);
};


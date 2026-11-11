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
	virtual ~SceneObject() = default;
	virtual bool hit(const Rectangle&);
	virtual void render() const override;
	virtual void saveValues(std::ostream& save);
	virtual void loadValues(std::istream& load);
};


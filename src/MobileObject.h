#pragma once
#include "SceneObject.h"

class Texture;
class Game;

class MobileObject : public SceneObject
{
protected:
	Vector2D<> vel;
	float animationTimer = 0.0f;
	int col = 0;
	int fil = 0;

public:
	MobileObject(Rectangle posDim, Texture* textura, Game* game, Vector2D<>vel, int fil=0);
	virtual ~MobileObject() = default;
	void render() const override;
	bool update(float dt) override;
	void loadValues(std::istream& load) override;

	void changeFil();

};


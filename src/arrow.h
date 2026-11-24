#pragma once
#include "game.h"
#include "rectangle.h"
#include "vector2D.h"
#include "MobileObject.h"

class PlayState;
class Texture;

class arrow : public MobileObject
{
private:
	int combo = 0;

public:
	arrow(Rectangle posDim, Texture* textura, PlayState* game, Vector2D<>vel);
	~arrow();
	bool update(float delta) override;
	void saveValues(std::ostream& save) override;
	void loadValues(std::istream& load) override;
};


#pragma once
#include "rectangle.h"
#include "vector2D.h"
#include "MobileObject.h"

class PlayState;

class balloon : public MobileObject
{
private:
	int fil;  // color
	bool dead = false;

public:
	balloon(Rectangle posDim, Vector2D<>vel, Texture* textura, int fil, PlayState* game);
	~balloon();

	bool update(float delta) override;
	bool hit(const Rectangle& obj) override;
	void saveValues(std::ostream& save) override;
	void loadValues(std::istream& load) override;
};


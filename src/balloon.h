#pragma once
#include "rectangle.h"
#include "vector2D.h"
#include "MobileObject.h"

class balloon : public MobileObject
{
private:
	int color;
	bool dead = false;

public:
	balloon(Rectangle posDim, Vector2D<>vel, Texture* textura, int color, Game* game);
	~balloon();

	bool update(float delta) override;
	bool hit(const Rectangle& obj) override;
	void saveValues(std::ostream& save) override;
};


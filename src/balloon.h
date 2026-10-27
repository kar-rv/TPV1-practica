#pragma once
#include "rectangle.h"
#include "vector2D.h"
#include "MobileObject.h"

class balloon : public MobileObject
{
private:
	int color;
	bool dead = false;
	float animationTimer = 0.0f;
	int row = 0;

public:
	balloon(Rectangle posDim, Vector2D<>vel, Texture* textura, int color, Game* game);
	~balloon();

	void render() const override;
	bool update(float delta) override;
	bool hit(const Rectangle& obj) override;
};


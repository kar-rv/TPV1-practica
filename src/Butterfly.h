#pragma once
#include <random>
#include "MobileObject.h"


class Butterfly: public MobileObject
{
public:
	Butterfly(Rectangle posDim, Vector2D<>vel, Texture* textura, Game* game);
	~Butterfly();

	bool update(float delta) override;
	bool hit(const Rectangle& obj) override;

private:
	bool dead = false;
	int probCambiar = 200;
	static std::mt19937_64 random;
};


#pragma once
#include "MobileObject.h"

class Butterfly: public MobileObject
{
public:
	Butterfly(Rectangle posDim, Vector2D<>vel, Texture* textura, Game* game);
	~Butterfly();

	bool update(float delta) override;
	
};


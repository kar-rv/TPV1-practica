#pragma once
#include "rectangle.h"
#include "vector2D.h"

class Texture;

class balloon
{
private:
	Rectangle posDim;
	int color;
	Vector2D<> vel;
	bool hit(const Rectangle& obj);

	Texture* textura;

public:
	balloon(Rectangle posDim, Vector2D<>vel, int color);
	~balloon();

	void render() const;
	bool update(float delta);
};


#pragma once
#include "rectangle.h"
#include "vector2D.h"

class Texture;

class balloon
{
private:
	Rectangle posDim;
	int color;
	bool dead = false;
	Vector2D<> vel;
	float animationTimer = 0.0f;
	int row = 1;

	Texture* textura;

	int ancho;
	int alto;
	int filas;
	int columnas;

public:
	balloon(Rectangle posDim, Vector2D<>vel, Texture* textura, int color);
	~balloon();

	void render() const;
	bool update(float delta);
	bool hit(const Rectangle& obj);
	bool animate(float delta);
};


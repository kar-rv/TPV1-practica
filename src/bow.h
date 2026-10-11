#pragma once
#include "rectangle.h"
#include "vector2D.h"
#include <SDL3/SDL_events.h>

class Texture;

class bow
{
private:
	Rectangle posDim;
	Vector2D<> vel;
	int numFlechas;
	bool armado;
	void handleEvent(const SDL_Event& event);

	Texture* textura;

public:
	void render() const;
	bool update(float delta);
};


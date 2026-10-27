#pragma once
#include <iostream>
#include "rectangle.h"
#include "vector2D.h"
#include "game.h"
#include "SceneObject.h"
#include <SDL3/SDL_events.h>

class bow: public SceneObject
{
private:
	Vector2D<> vel;
	int numFlechas;
	bool armado;

public:
	bow(Rectangle posDim, Vector2D<> vel, Texture* textura, int numFlechas, bool armado = false, Game* game = nullptr);
	~bow();
	void render() const override;
	bool update(float dt) override;
	void handleEvent(const SDL_Event& event);

	int getNumFlechas() const {
		return numFlechas;
	}
};


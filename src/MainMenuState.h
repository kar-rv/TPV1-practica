#pragma once

#include <SDL3/SDL.h>
#include "GameState.h"
#include "rectangle.h"

class MainMenuState : public GameState
{
private:
	Rectangle nuevaPartida;
	Rectangle cargarPartida;
	Rectangle salir;

	bool isPointInside(float x, float y, const Rectangle& rect) const;

public:
	MainMenuState(Game* game);
	~MainMenuState() override = default;

	bool update(float delta) override;
	void render() const override;
	void handleEvent(const SDL_Event& event) override;
};

#pragma once
#include "GameState.h"
#include "Rectangle.h"

class PlayState;

class PauseState: public GameState
{
private:
	Rectangle continuar;
	Rectangle guardarPartida;
	Rectangle cargarPartida;
	Rectangle volverAlMenu;
	Rectangle salir;

	PlayState* playState = nullptr;

	bool isPointInside(float x, float y, const Rectangle& rect) const;

public:
	PauseState(Game* game, PlayState* playState);
	~PauseState() override = default;

	bool update(float delta) override;
	void render() const override;
	void handleEvent(const SDL_Event& event) override;
};


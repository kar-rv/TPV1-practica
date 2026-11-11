#pragma once

class Game;

class GameObject
{
public:
	GameObject(Game* game);
	virtual ~GameObject() = default;
	virtual void render() const = 0;
	virtual bool update(float dt) = 0;

protected:
	Game* game;
};


#pragma once
#include <list>
#include <SDL3/SDL.h>
#include "EventHandler.h"

class Game;
class GameObject;

class GameState
{
protected:
    std::list<GameObject*> gameObjects;
    std::list<EventHandler*> eventHandlers;
    Game* game = nullptr;

public:
    explicit GameState(Game* g);
    virtual ~GameState();

    virtual void render() const;
    virtual bool update(float delta);
    virtual void handleEvent(const SDL_Event& event);

    virtual void addEventListener(EventHandler* listener);
    virtual void addObject(GameObject* obj);

    Game* getGame() const {
        return game;
    }
};


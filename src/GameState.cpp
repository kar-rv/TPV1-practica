#include "GameState.h"
#include "GameObject.h"

GameState::GameState(Game* g) : game(g) {

}

GameState::~GameState()
{
    for (GameObject* obj : gameObjects) {
        delete obj;
    }
    gameObjects.clear();
    eventHandlers.clear();
}

void GameState::addEventListener(EventHandler* listener)
{
    if (listener != nullptr) {
        eventHandlers.push_back(listener);
    }
}

void GameState::addObject(GameObject* obj)
{
    if (obj != nullptr) {
        gameObjects.push_back(obj);
    }
}

void GameState::handleEvent(const SDL_Event& event)
{
    for (EventHandler* handler : eventHandlers) {
        if (handler != nullptr) {
            handler->handleEvent(event);
        }
    }
}

bool GameState::update(float delta)
{
    for (GameObject* obj : gameObjects) {
        if (obj != nullptr) {
            obj->update(delta);
        }
    }
    return true;
}

void GameState::render() const
{
    for (GameObject* obj : gameObjects) {
        if (obj != nullptr) {
            obj->render();
        }
    }
}

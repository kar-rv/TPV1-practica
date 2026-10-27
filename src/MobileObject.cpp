#include "MobileObject.h"
#include "game.h"
#include "texture.h"

MobileObject::MobileObject(Rectangle posDim, Texture* textura, Game* game, Vector2D<>vel) :
	SceneObject(posDim, textura, game), vel(vel) {
}

bool
MobileObject::update(float delta) {
	return false;
}


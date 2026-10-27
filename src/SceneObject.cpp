#include "SceneObject.h"
#include "game.h"
#include "texture.h"

SceneObject::SceneObject(Rectangle posDim, Texture* textura, Game* game) :
	GameObject(game), posDim(posDim), textura(textura) {

}
bool SceneObject::hit(const Rectangle& other)
{
	// Comprobación básica de intersección de rectángulos usando el método de Rectangle
	return posDim.hasIntersection(other);
}

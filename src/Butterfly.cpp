#include "Butterfly.h"
#include "game.h"

std::mt19937_64 Butterfly::random(std::random_device{}());

Butterfly::Butterfly(Rectangle posDim, Vector2D<>vel, Texture* textura, Game* game) :
	MobileObject(posDim, textura, game, vel){

}

Butterfly::~Butterfly() {

}

bool
Butterfly::update(float delta) {
    float x = posDim.getTopLeftCorner().getX();
    float y = posDim.getTopLeftCorner().getY();
    float width = posDim.getWidth();
    float height = posDim.getHeight();

    if (dead) {
        posDim.move(Vector2D<>(vel.getX() * delta, vel.getY() * delta));
        if (y > Game::WINDOW_HEIGHT) {
            std::cout << "muerto" << std::endl;
            return true;
        }
    }
    else {
        if (std::uniform_int_distribution<int>(0, probCambiar + 1)(random) == probCambiar) {
            vel.invertX();
            vel.invertY();
        }

        // Límites en el eje X
        if (x < 0 && vel.getX() < 0) {
            vel.invertX();
        }
        else if (x > Game::WINDOW_WIDTH - width && vel.getX() > 0) {
            vel.invertX();
        }

        // Límites en el eje Y
        if (y < 0 && vel.getY() < 0) {
            vel.invertY();
        }
        else if (y > Game::WINDOW_HEIGHT - height && vel.getY() > 0) {
            vel.invertY();
        }

        posDim.move(Vector2D<>(vel.getX() * delta, vel.getY() * delta));
    }

    return false;
}

bool
Butterfly::hit(const Rectangle& obj) {
    if (!dead && posDim.hasIntersection(obj)) {
        //row = 1;
        dead = true;
        vel = Vector2D<>(0, 250);
        return true;
    }
    return false;
}

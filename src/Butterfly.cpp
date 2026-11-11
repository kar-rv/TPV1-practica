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
        //la maroposa cae hacia abajo
        posDim.move(Vector2D<>(vel.getX() * delta, vel.getY() * delta));

        //se elimina si lleva al limite de la pantalla
        if (y > Game::WINDOW_HEIGHT) {
            std::cout << "muerto" << std::endl;
            return true;
        }
    }
    else {
        //animación
        if (vel.getX() >= 0) {
            fil = 1;
        }
        else
            fil = 0;
        animationTimer += delta;
        if (animationTimer > 0.05f) {
            col++;
            animationTimer = 0.0f;
        }
        if (col > 6) {
            col = 0;
        }

        //vemos si tiene que cambiar de dirección
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
        dead = true;
        vel = Vector2D<>(0, 250);

        //actualizamos el score del game
        int newScore = game->getScore() + game->getButPoints();
        //el score no puede ser negativo, tope en 0
        if (newScore <= 0) 
            game->changeScore(-game->getScore());
        else
        game->changeScore(game->getButPoints());

        return true;
    }
    return false;
}

void
Butterfly::saveValues(std::ostream& save) {
    char n = 'n';
    if (dead) n = 'm';
    // por orden se guarda: posicion(x, y), dimensiones(w, h), velocidad(u, v), muerta(m -> si / n -> no)
    save << "butterfly " << posDim.getTopLeftCorner().getX() << " " << posDim.getTopLeftCorner().getY() << " "
        << posDim.getWidth() << " " << posDim.getHeight() << " " << vel.getX() << " " << vel.getY() << " "
        << n << " " << std:: endl;
}

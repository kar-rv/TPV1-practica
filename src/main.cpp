//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"

int main(int argc, char* argv[])
{
	try {
		Game().run();
	}
	catch (const std::exception& e) {
		// devuelve mensaje explicativo sobre el error capturado
		std::cerr << "Excepcion capturada: " << e.what() << std::endl;
	}
	return 0;
}

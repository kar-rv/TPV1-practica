//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"
#include <string>

int main(int argc, char* argv[])
{
	try {
		Game().run();
	}
	catch (const std::string& e) {
		std::cerr << "Excepcion capturada: " << e << std::endl;
		return 1;
	}
	return 0;
}

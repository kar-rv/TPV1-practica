//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"
#include <string>
#include "GameError.h"

using namespace std;

int main(int argc, char* argv[]) {
	try {
		Game().run();
	}
	catch (const GameError& e) {
		cerr << e.what() << endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
		return 1;
	}
	catch (const exception& e) {
		cerr << e.what() << endl;
		return 1;
	}
	return 0;
}

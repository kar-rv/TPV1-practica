#include "game.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>

#include <SDL3_image/SDL_image.h>

#include "texture.h"
#include "bow.h"
#include "balloon.h"
#include "rectangle.h"
#include "arrow.h"
#include "infoBar.h"

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
	
	TextureSpec{"bow.png", 1, 2},
	TextureSpec{"arrow.png"},
	TextureSpec{"balloons.png", 7, 6},
	TextureSpec{"background1.png"}
};

Game::Game()
  : exit(false), random(time(nullptr))
{
	// carga el archivo de niveles
	int lvl = 0;
	do {
		cout << "¿Level 1 or 2?" << '\n';
		cin >> lvl;
	} while (lvl != 1 && lvl != 2);
	string route = LEVEL_ROOT + to_string(lvl) + ".txt";

	try {
		if (!loadLevel(route)) {  // este método devuelve true si se ha podido cargar el nivel y false si no
			throw "No se ha podido abrir el archivo de niveles";
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Excepcion capturada: " << e.what() << std::endl;
	}

	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw "SDL_Init: "s + SDL_GetError();

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
	                                 WINDOW_WIDTH, WINDOW_HEIGHT,
	                                 0, &window, &renderer))
		throw "window or renderer: "s + SDL_GetError();
	size_t i = 0;
	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;

		for (i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
	}
	catch (...) {
		// TODO: liberar la memoria reservada
		for (size_t j = 0; j < i; j++) {
			delete textures[j];
			textures[j] = nullptr;
		}
		throw "Error al cargar las texturas"s; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// TODO: crear los objetos del juego
	Rectangle rect(Point2D<>(25, 100), 100, 100);
	bowPtr = new bow(rect, Vector2D<>(0, 10), textures[BOW], numArrows, false, this);
	infoBarPtr = new infoBar(renderer, textures[ARROW]);
}

Game::~Game()
{
	// se libera la memoria de todos los objetos
	for (arrow* arrow : arrows) delete arrow;
	for (balloon* balloon : balloons) delete balloon;
	for (Texture* texture : textures) delete texture;
	delete bowPtr;
	delete infoBarPtr;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void
Game::render() const
{
	SDL_RenderClear(renderer);

	textures[BACKGROUND]->render(); // Llama al método de Texture que dibuja a pantalla completa
	
	// se renderiza el arco
	bowPtr->render();

	// se renderizan los globos
	for (auto b : balloons) {
		b->render();
	}

	// se renderizan las flechas
	for (auto a : arrows)
		a->render();

	// se renderiza la barra de información
	infoBarPtr->render(bowPtr->getNumFlechas(), score);

	// se llama al renderer
	SDL_RenderPresent(renderer);
}

void
Game::update(float delta)
{
	// temporizador para generar globos
	static float timeSinceLastBalloonSpawn = 0.0f;
	timeSinceLastBalloonSpawn += delta;

	// generar un globo cada 2
	if (timeSinceLastBalloonSpawn > balloonDelay) {
		generateBalloon();
		timeSinceLastBalloonSpawn = 0.0f;
	}

	// actualizamos los glovos y los eliminamos si es necesario
	for (auto b = balloons.begin(); b != balloons.end(); ) {
		if ((*b)->update(delta)) {
			delete* b;
			b = balloons.erase(b);
			cout << "borrando globo" << endl;
		}
		else ++b;
	}

	// actualizamos las flechas tambien
	for (auto a = arrows.begin(); a != arrows.end(); ) {
		if (!(*a)->update(delta)) {
			delete* a;
			a = arrows.erase(a);
		}
		else ++a;
	}
	

}

void
Game::run()
{
	while (!exit) {
		// calcular el Delta Time en segundos
		Uint64 currentTime = SDL_GetTicks();
		float delta = (currentTime - lastTime) / 1000.0f; // Pasamos milisegundos a segundos
		lastTime = currentTime;

		// se limita el delta por si la ventana se arrastra o se congela
		if (delta > 0.1f) delta = 0.1f;

		render();
		update(delta);
		handleEvents();

		// se espera lo que falte hasta completar el periodo del frame
		Uint64 elapsed = SDL_GetTicks() - currentTime;
		if (elapsed < FRAME_PERIOD)
			SDL_Delay(FRAME_PERIOD - elapsed);
	}
}

void
Game::handleEvents()
{
	SDL_Event event;

	// Only quit is handled directly, everything else is delegated
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;
		bowPtr->handleEvent(event);

		// TODO
	}
}

void 
Game::generateBalloon() {
	// valores random
	int color = uniform_int_distribution<int>(0, 6)(random);
	int posX = uniform_int_distribution<int>(200, 700)(random);
	float velY = uniform_int_distribution<int>(50, 200)(random);
	Rectangle rect(Point2D<>(posX, 600), 100, 100);

	// se genera el globo con los valores random
	balloons.push_back(new balloon(rect, Vector2D<>(0, -velY), textures[BALLOON], color));

	// este switch es solo para debug
	cout << "generando globo: ";
	switch (color) {
	case 0:
		cout << "azul";
			break;
	case 1:
		cout << "verde";
		break;
	case 2:
		cout << "naranja";
		break;
	case 3:
		cout << "rojo";
		break;
	case 4:
		cout << "amarillo";
		break;
	case 5:
		cout << "rosa";
		break;
	case 6:
		cout << "morado";
		break;
	}
	cout << "  y vel: " << velY << endl;
}

void
Game::generateArrow(const Point2D<>& pos) {
	Texture* arrowTexture = textures[ARROW];

	// esto es para que el arrow se genere en mitad del arco y no en la esquina de arriba
	float arrowHeight = 100 / arrowTexture->getAspectRatio();
	Rectangle rect(pos, 100, arrowHeight);

	// se genera la flecha
	arrows.push_back(new arrow(rect, Vector2D<>(300, 0), arrowTexture, this));
	cout << "generando flecha" << endl;
}

 bool
 Game::checkCollision(const Rectangle& rect)
 {
	 bool colision = false;
	 // se recorre el array de globos
	 for (auto b = balloons.begin(); b != balloons.end(); ) {
		 // si un globo ha colisionado con una flecha se marca que ha habido colision
		 if ((*b)->hit(rect)) {
			colision = true;
			score += 10;
		 }
		 else ++b;
	 }

	return colision;
 }

 bool
 Game::loadLevel(string route)
 {
	 ifstream levels(route);
	 if (!levels.is_open()) {
		 // cerr es como cout pero se utiliza cuando se escribe un mensaje de error (aunque funcionan igual)
		 throw "Error: No se pudo abrir el archivo con ruta (" + route + ")"s;
		 //cerr << "No se pudo abrir el archivo con ruta " << route << '\n';
		 return false;
	 }

	 string word;
	 while (levels >> word) {
		 if (word == "arrows") {
			 levels >> numArrows;  // número de flechas
		 }
		 else if (word == "baldelay") {
			 levels >> balloonDelay;  // delay del globo
			 balloonDelay = balloonDelay / 1000.0f;  // se pasan milisegundos a segundos
		 }
		 else {
			 // si es cualquier otra palabra se ignora
			 levels.ignore(numeric_limits<streamsize>::max(), '\n');
		 }
	 }
	 return true;
 }

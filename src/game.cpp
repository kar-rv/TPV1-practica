#include "game.h"
#include <iostream>
#include <string>

#include <SDL3_image/SDL_image.h>

#include "texture.h"
#include "bow.h"
#include "balloon.h"
#include "rectangle.h"
#include "arrow.h"

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
	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw "SDL_Init: "s + SDL_GetError();

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
	                                 WINDOW_WIDTH, WINDOW_HEIGHT,
	                                 0, &window, &renderer))
		throw "window or renderer: "s + SDL_GetError();

	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;

		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
	}
	catch (...) {
		// TODO: liberar la memoria reservada
		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// TODO: crear los objetos del juego
	Rectangle rect(Point2D<>(25, 100), 100, 100);
	bowPtr = new bow(rect, Vector2D<>(0, 10), textures[BOW], 10, false, this);
}

Game::~Game()
{
	// TODO: liberar la memoria reservada por la clase
	for (arrow* arrow : arrows) delete arrow;
	for (balloon* balloon : balloons) delete balloon;
	for (Texture* texture : textures) delete texture;
	delete bowPtr;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void
Game::render() const
{
	SDL_RenderClear(renderer);

	// TODO
	textures[BACKGROUND]->render(); // Llama al método de Texture que dibuja a pantalla completa
	
	bowPtr->render();

	for (auto b : balloons) {
		b->render();
	}

	for (auto a : arrows)
		a->render();

	SDL_RenderPresent(renderer);
}

void
Game::update(float delta)
{
	// temporizador para generar globos
	static float timeSinceLastBalloonSpawn = 0.0f;
	timeSinceLastBalloonSpawn += delta;

	// generar un globo cada 2
	if (timeSinceLastBalloonSpawn > 2.0f) {
		generateBalloon();
		timeSinceLastBalloonSpawn = 0.0f;
	}

	// actualizamos los glovos y los eliminamos si es necesario
	auto b = balloons.begin();
	while (b != balloons.end()) {
		// *b nos da el puntero al balloon actual
		if ((*b)->update(delta)) {
			delete* b;
			b = balloons.erase(b);
			cout << "borrando globo" << endl;
		}
		// si sigue vivo, avanzamos al siguiente
		else 
			++b;
	}

	// esto se me genera un poco a trompicones
	for (auto a = arrows.begin(); a != arrows.end(); ) {
		if (!(*a)->update(1.0)) {   // delta fijo de momento
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
		// TODO: implementar bucle del juego
		// Calcular el Delta Time en segundos
		Uint64 currentTime = SDL_GetTicks();
		float delta = (currentTime - lastTime) / 1000.0f; // Pasamos milisegundos a segundos
		lastTime = currentTime;

		// Limitar el delta por seguridad si la ventana se arrastra o se congela
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
	int posX = uniform_int_distribution<int>(100, 700)(random);
	float velY = uniform_int_distribution<int>(50, 200)(random);
	Rectangle rect(Point2D<>(posX, 600), 100, 100);

	balloons.push_back(new balloon(rect, Vector2D<>(0, -velY), textures[BALLOON], color));

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
	float arrowHeight = 100 / arrowTexture->getAspectRatio();
	Rectangle rect(pos, 100, arrowHeight);
	arrows.push_back(new arrow(rect, Vector2D<>(10, 0), arrowTexture));
	cout << "generando flecha" << endl;
}

// bool
// Game::checkCollision(const Rectangle& rect)
// {
//	// TODO
//	return false;
// }

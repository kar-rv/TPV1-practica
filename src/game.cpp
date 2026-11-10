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
#include "Butterfly.h"

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
	TextureSpec{"butterfly.png", 4, 10}
};

Game::Game()
  : exit(false), random(time(nullptr))
{
	// carga el archivo de niveles
	lvlActual = 0;
	do {
		cout << "¿Level 1, 2 or 3?" << '\n';
		cin >> lvlActual;
	} while (lvlActual != 1 && lvlActual != 2 && lvlActual!=3);
	string route = LEVEL_ROOT + to_string(lvlActual) + ".txt";

	if (!loadLevel(route)) {  // este método devuelve true si se ha podido cargar el nivel y false si no
		throw "No se ha podido abrir el archivo de niveles";
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

	// crear los objetos del juego
	generateNewLevel();
}

Game::~Game()
{
	// se libera la memoria de todos los objetos
	for (GameObject* object : objects) {
		delete object;
	}
	objects.clear();
	for (Texture* texture : textures) {
		delete texture;
	}

	if (bowPtr != nullptr) {
		delete bowPtr;
		bowPtr = nullptr;
	}
	if (infoBarPtr != nullptr) {
		delete infoBarPtr;
		infoBarPtr = nullptr;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void
Game::render() const
{
	if (juegoTerminado) {
		const float scale = 5.0f;
		SDL_SetRenderScale(renderer, scale, scale);
		//color en negro y limpiamos la pantalla
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Cambiamos el color de dibujo a blanco (para que el texto sea visible)
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		// Pintamos el texto (puedes ajustar las coordenadas X e Y para centrarlo)
		string text = "HAS GANADO";
		SDL_RenderDebugText(renderer, 40, 55, text.c_str());

		SDL_RenderPresent(renderer);
		return;
	}

	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderClear(renderer);
	
	bowPtr->render();
	// se renderizan los globos
	if (!objects.empty()) {
		for (auto object : objects) {
			object->render();
		}
	}

	infoBarPtr->render();

	// se llama al renderer
	SDL_RenderPresent(renderer);
}

void
Game::update(float delta)
{
	if (juegoTerminado) return;
	// temporizador para generar globos
	static float timeSinceLastBalloonSpawn = 0.0f;
	timeSinceLastBalloonSpawn += delta;

	// generamos los globos
	if (timeSinceLastBalloonSpawn > balloonDelay) {
		generateBalloon();
		timeSinceLastBalloonSpawn = 0.0f;
	}

	infoBarPtr->update(delta);
	bowPtr->update(delta);

	// actualizamos los objetos y los eliminamos si es necesario
	for (auto object = objects.begin(); object != objects.end(); ) {
		if ((*object)->update(delta)) {
			delete* object;
			object = objects.erase(object);
			cout << "borrando globo" << endl;
		}
		else ++object;
	}
	// vemos si hemos logrado el goal del nivel para pasar al siguiente
	checkGoal();
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
	}
}

void 
Game::generateBalloon() {
	// valores random
	int color = uniform_int_distribution<int>(0, 6)(random);
	int posX = uniform_int_distribution<int>(200, 700)(random);
	float velY = uniform_int_distribution<int>(200, 500)(random) * balSpeed;
	Rectangle rect(Point2D<>(posX, 600), 100, 100);

	// se genera el globo con los valores random
	balloon* Balloon= new balloon(rect, Vector2D<>(0, -velY), textures[BALLOON], color, this);
	objects.push_back(Balloon);
}

void
Game::generateArrow(const Point2D<>& pos) {
	Texture* arrowTexture = textures[ARROW];

	// esto es para que el arrow se genere en mitad del arco y no en la esquina de arriba
	float arrowHeight = 100 / arrowTexture->getAspectRatio();
	Rectangle rect(pos, 100, arrowHeight);

	// se genera la flecha
	objects.push_back(new arrow(rect, arrowTexture, this, Vector2D<>(300, 0)));
	cout << "generando flecha" << endl;
}

 bool
 Game::checkCollision(const Rectangle& rect)
 {
	 bool colision = false;
	 // se recorre el array de objetos
	 for (auto object = objects.begin(); object != objects.end(); ++object) {
		 if ((*object)->hit(rect)) { //el hit de cada objeto se encarga de actualizar el score
			 colision = true;
		 }
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
		 if (word == "bgcolor") {
			 levels >> r >> g >> b; //colores del background
		 }
		 else if (word == "arrows") {
			 levels >> numArrows;  // número de flechas
		 }
		 else if (word == "baldelay") {
			 levels >> balloonDelay;  // delay del globo
			 balloonDelay = balloonDelay / 1000.0f;  // se pasan milisegundos a segundos
		 }
		 else if (word == "balpoints") {
			 levels >> balPoints; //puntuacion por globo
		 }
		 else if (word == "balspeed") {
			 levels >> balSpeed; //velocidad de los globos
		 }
		 else if (word == "goal") {
			 levels >> goal; //puntuación de victoria
		 }
		 else if (word == "butterflies") {
			 levels >> numButterflies;  // número de flechas
		 }
		 
		 else {
			 // si es cualquier otra palabra se ignora
			 levels.ignore(numeric_limits<streamsize>::max(), '\n');
		 }
	 }
	 return true;
 }

 void
Game::generarButterflies() {
	 for (int i = 0; i < numButterflies; i++) {
		 int posX = uniform_int_distribution<int>(0, WINDOW_WIDTH - 75)(random);
		 int posY = uniform_int_distribution<int>(0, WINDOW_HEIGHT - 75)(random);
		 float velX = uniform_int_distribution<int>(-100, 100)(random);
		 float velY = uniform_int_distribution<int>(-100, 100)(random);

		 Rectangle rect(Point2D<>(posX, posY), 75, 75);
		 objects.push_back(new Butterfly(rect, Vector2D<>(velX, velY), textures[BUTTERFLY], this));
	 }
 }

void
Game::checkGoal() {
	if (score >= goal) {
		int siguienteNivel = lvlActual + 1;
		if (siguienteNivel == 4){
			end();
			return;
		}
		string route = LEVEL_ROOT + to_string(siguienteNivel) + ".txt";

		// Actualizamos el nivel actual antes de cargar
		lvlActual = siguienteNivel;

		// Intentamos cargar el archivo del nuevo nivel
		if (!loadLevel(route)) {
			throw "No se ha podido abrir el archivo de niveles";
		}
		else {
			generateNewLevel();
		}
	}
}

void
Game::generateNewLevel() {
	score = 140;
	cout << "level: " << lvlActual << endl;
	cout << "goal: " << goal << endl;
	cout << "butterflies: " << numButterflies << endl;

	//blorramos los objetos del nivel anterior
	for (GameObject* object : objects) delete object;
	objects.clear();

	//creamos nuevos objetos
	Rectangle rect(Point2D<>(25, 100), 100, 100);
	bowPtr = new bow(rect, Vector2D<>(0, 10), textures[BOW], numArrows, false, this);
	infoBarPtr = new infoBar(renderer, textures[ARROW], this);

	generarButterflies();
}

void
Game::end() {
	juegoTerminado = true;

	//Borramos todos los objetos de la pantalla de forma segura
	for (GameObject* object : objects) {
		delete object;
	}
	objects.clear();

	//Borramos bowPtr y lo ponemos a nullptr para evitar dobles borrados
	if (bowPtr != nullptr) {
		delete bowPtr;
		bowPtr = nullptr;
	}

	//Borramos infoBarPtr y lo ponemos a nullptr
	if (infoBarPtr != nullptr) {
		delete infoBarPtr;
		infoBarPtr = nullptr;
	}
}

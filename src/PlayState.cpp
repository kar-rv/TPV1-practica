#include "PlayState.h"

#include "texture.h"
#include "bow.h"
#include "balloon.h"
#include "rectangle.h"
#include "arrow.h"
#include "infoBar.h"
#include "Butterfly.h"
#include "GameError.h"

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <sstream>

using namespace std;

// Constantes
constexpr const char* LEVEL_ROOT = "../assets/levels/";
constexpr const char* SAVES_ROOT = "../assets/saves/saved.txt";
constexpr int FIRST_LEVEL = 3;

PlayState::PlayState(Game* game) : GameState(game) {

}
PlayState::~PlayState() {
	// se libera la memoria de todos los objetos
	for (GameObject* object : objects) {
		delete object;
	}
	objects.clear();

	if (bowPtr != nullptr) {
		delete bowPtr;
		bowPtr = nullptr;
	}
	if (infoBarPtr != nullptr) {
		delete infoBarPtr;
		infoBarPtr = nullptr;
	}
}

void
PlayState::render() const
{
	//pantalla de cuando se ha acabado el juego
	if (juegoTerminado) {
		//escala del texto
		const float scale = 5.0f;
		SDL_SetRenderScale(game->getRenderer(), scale, scale);

		//color en negro y limpiamos la pantalla
		SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 255);
		SDL_RenderClear(game->getRenderer());

		//cambiamos el color de dibujo a blanco
		SDL_SetRenderDrawColor(game->getRenderer(), 255, 255, 255, 255);

		//pintamos el texto
		string text = "HAS GANADO";
		if (juegoPerdido) {
			text = "HAS PERDIDO";
		}

		SDL_RenderDebugText(game->getRenderer(), 40, 55, text.c_str());

		SDL_RenderPresent(game->getRenderer());
	}
	else {
		SDL_SetRenderDrawColor(game->getRenderer(), r, g, b, 255);
		SDL_RenderClear(game->getRenderer());

		bowPtr->render();
		// se renderizan los globos
		if (!objects.empty()) {
			for (auto object : objects) {
				object->render();
			}
		}

		infoBarPtr->render();

		// se llama al renderer
		SDL_RenderPresent(game->getRenderer());
	}
}

bool
PlayState::update(float delta)
{
	if (juegoTerminado) return false;
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
	checkDefeat();
	return false;
}

void
PlayState::handleEvents()
{
	SDL_Event event;

	// Only quit is handled directly, everything else is delegated
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;

		if (!juegoTerminado) {  // mientras el juego no haya terminado
			bowPtr->handleEvent(event);

			// si no es una pulsación de tecla, no se hace nada
			if (event.type == SDL_EVENT_KEY_DOWN) {
				bool control = (event.key.mod & SDL_KMOD_CTRL) != 0;
				if (control && event.key.key == SDLK_S && !event.key.repeat) {
					// !event.key.repeat evita que se ejecute todo el rato al mantenerlo pulsado
					// se guardan los datos del juego actual
					try {
						saveLevel(SAVES_ROOT);
					}
					catch (const GameError& e) {
						cerr << e.what() << endl;  // no se guardo
					}
				}

				if (control && event.key.key == SDLK_O && !event.key.repeat) {
					try {
						loadSavedLevel(SAVES_ROOT);
					}
					catch (const GameError& e) {
						cerr << e.what() << endl;   // se sigue jugando la actual si la partida no ha cargado bien
					}
				}
			}
		}
	}
}

void
PlayState::generateBalloon() {
	// valores random
	int color = uniform_int_distribution<int>(0, 6)(random);
	int posX = uniform_int_distribution<int>(200, 700)(random);
	float velY = uniform_int_distribution<int>(200, 500)(random) * balSpeed;
	Rectangle rect(Point2D<>(posX, 600), 100, 100);

	// se genera el globo con los valores random
	balloon* Balloon = new balloon(rect, Vector2D<>(0, -velY), game->getTexture(Game::BALLOON), color, this);
	objects.push_back(Balloon);
}

void
PlayState::generateArrow(const Point2D<>& pos) {
	Texture* arrowTexture = game->getTexture(Game::ARROW);

	// esto es para que el arrow se genere en mitad del arco y no en la esquina de arriba
	float arrowHeight = 100 / arrowTexture->getAspectRatio();
	Rectangle rect(pos, 100, arrowHeight);

	// se genera la flecha
	objects.push_back(new arrow(rect, arrowTexture, this, Vector2D<>(300, 0)));
	cout << "generando flecha" << endl;
}

bool
PlayState::checkCollision(const Rectangle& rect)
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

string
PlayState::levelPath(int level) const
{
	return LEVEL_ROOT + to_string(level) + ".txt";
}

bool
PlayState::levelExists(int level) const
{
	return ifstream(levelPath(level)).is_open();
}

void
PlayState::loadLevel(const string& route)
{
	ifstream levels(route);
	if (!levels.is_open()) {
		throw FileNotFoundError(route);
	}

	string line;
	int lineNum = 0;
	// getline es para saber los saltos de linea que lleva el archivo
	while (getline(levels, line)) {  // mientras haya lineas en el archivo
		++lineNum;
		// flujo de entrada que lee de un string y funciona igual que cin o un ifstream
		istringstream streamLine(line);
		string word;
		// las líneas vacías se ignoran
		if (streamLine >> word) parseLevelLine(route, lineNum, word, streamLine);
	}
}

void
PlayState::parseLevelLine(const string& route, int lineNum, const string& word, istream& streamLine)
{
	if (word == "bgcolor") {
		if (!(streamLine >> r >> g >> b))
			throw FileFormatError(route, lineNum, "bgcolor necesita tres enteros (r g b)");
	}
	else if (word == "arrows") {
		if (!(streamLine >> numArrows) || numArrows < 0)
			throw FileFormatError(route, lineNum, "arrows necesita un entero no negativo");
	}
	else if (word == "baldelay") {
		if (!(streamLine >> balloonDelay) || balloonDelay <= 0)
			throw FileFormatError(route, lineNum, "baldelay necesita un valor positivo");
		balloonDelay = balloonDelay / 1000.0f;  // se pasan milisegundos a segundos
	}
	else if (word == "balpoints") {
		if (!(streamLine >> balPoints))
			throw FileFormatError(route, lineNum, "balpoints necesita un entero");
	}
	else if (word == "balspeed") {
		if (!(streamLine >> balSpeed))
			throw FileFormatError(route, lineNum, "balspeed necesita un número");
	}
	else if (word == "goal") {
		if (!(streamLine >> goal))
			throw FileFormatError(route, lineNum, "goal necesita un entero");
	}
	else if (word == "butterflies") {
		if (!(streamLine >> numButterflies) || numButterflies < 0)
			throw FileFormatError(route, lineNum, "butterflies necesita un entero no negativo");
	}
	// si es cualquier otra palabra se ignora
}

void
PlayState::generarButterflies() {
	for (int i = 0; i < numButterflies; i++) {
		int posX = uniform_int_distribution<int>(0, Game::WINDOW_WIDTH - 75)(random);
		int posY = uniform_int_distribution<int>(0, Game::WINDOW_HEIGHT - 75)(random);
		float velX = uniform_int_distribution<int>(-100, 100)(random);
		float velY = uniform_int_distribution<int>(-100, 100)(random);

		Rectangle rect(Point2D<>(posX, posY), 75, 75);
		objects.push_back(new Butterfly(rect, Vector2D<>(velX, velY), game->getTexture(Game::BUTTERFLY), this));
	}
}

void
PlayState::checkGoal() {
	if (goal <= 0 || score < goal) return;
	const int nextLevel = lvlActual + 1;
	if (!levelExists(nextLevel)) {
		end();
	}
	else {
		lvlActual = nextLevel;
		loadLevel(levelPath(nextLevel));
		generateNewLevel();
	}
}

void
PlayState::generateNewLevel() {
	score = 0;
	cout << "level: " << lvlActual << endl;
	cout << "goal: " << goal << endl;
	cout << "butterflies: " << numButterflies << endl;

	//blorramos los objetos del nivel anterior
	for (GameObject* object : objects) delete object;
	objects.clear();

	//creamos nuevos objetos
	Rectangle rect(Point2D<>(25, 100), 100, 100);
	// se borran el arco y la infobar antes de crear las nuevas
	delete bowPtr;
	delete infoBarPtr;
	bowPtr = new bow(rect, Vector2D<>(0, 10), game->getTexture(Game::BOW), numArrows, false, this);
	infoBarPtr = new infoBar(game->getRenderer(), game->getTexture(Game::ARROW), this);

	generarButterflies();
}

void
PlayState::saveLevel(string route) {
	ofstream save(route);
	if (!save) {
		throw FileNotFoundError(route);
	}

	save << lvlActual << " " << score << endl;
	bowPtr->saveValues(save);
	for (auto object = objects.begin(); object != objects.end(); ++object) {
		(*object)->saveValues(save);
	}
	cout << "Se ha guardado el archivo" << endl;
}

void
PlayState::loadSavedLevel(string route) {
	ifstream load(route);
	if (!load) {
		throw FileNotFoundError(route);
	}

	int lvl, pts;
	int lineNum = 1;
	readSavedHeader(load, route, lvl, pts);  // carga nivel y puntuacion

	list<SceneObject*> objs;
	bow* newBow = nullptr;

	try {
		// va leyendo una linea por objeto
		readSavedObjects(load, route, lineNum, objs, newBow);
		if (newBow == nullptr) {
			throw FileFormatError(route, lineNum, "el arco no se ha guardado");
		}
		// se carga el nivel (1, 2, 3...)
		loadLevel(levelPath(lvl));
	}
	// si falla la carga aqui se borra todo lo que se quedo a mitad para evitar memory leaks
	catch (...) {
		for (SceneObject* o : objs) delete o;
		delete newBow;
		throw;
	}

	// sustituye la partida actual por la cargada
	for (SceneObject* o : objects) delete o;
	objects = objs;
	delete bowPtr;
	bowPtr = newBow;
	lvlActual = lvl;
	score = pts;
	cout << "se ha cargado la partida" << endl;
}

void
PlayState::readSavedHeader(istream& load, const string& route, int& lvl, int& pts) {
	string line;
	if (!getline(load, line)) {
		throw FileFormatError(route, 1, "el archivo está vacío");
	}
	istringstream streamLine(line);
	if (!(streamLine >> lvl >> pts) || lvl < FIRST_LEVEL || !levelExists(lvl)) {
		throw FileFormatError(route, 1, "nivel o puntuacion incorrectas");
	}
}

void
PlayState::readSavedObjects(istream& load, const string& route, int& lineNum, list<SceneObject*>& objs, bow*& newBow) {
	string line;
	while (getline(load, line)) {
		++lineNum;
		istringstream streamLine(line);
		string word;
		if (streamLine >> word) {
			try {
				if (word == "bow") {
					if (newBow != nullptr) {
						throw FileFormatError(route, lineNum, "el arco aparece más de una vez");
					}
					// valores de relleno que luego se cargan con loadValues()
					newBow = new bow(Rectangle(Point2D<>(0, 0), 0, 0), Vector2D<>(0, 0), game->getTexture(Game::BOW), 0, false, this);
					newBow->loadValues(streamLine);
				}
				else {
					SceneObject* obj = createSavedObject(word);
					if (obj == nullptr) {
						throw FileFormatError(route, lineNum, "este objeto no existe (" + word + ")");
					}
					objs.push_back(obj);  // se guarda antes de cargar para poder liberarlo si falla
					obj->loadValues(streamLine);
				}
			}
			catch (const FileFormatError&) {
				throw;
			}
			catch (const GameError& e) {
				// los loadValues lanzan GameError sin conocer el archivo: se añade aquí
				throw FileFormatError(route, lineNum, e.what());
			}
		}
	}
}

SceneObject*
PlayState::createSavedObject(const string& word) {
	Rectangle rect(Point2D<>(0, 0), 0, 0);
	Vector2D<> vel(0, 0);

	if (word == "balloon") return new balloon(rect, vel, game->getTexture(Game::BALLOON), 0, this);
	if (word == "arrow") return new arrow(rect, game->getTexture(Game::ARROW), this, vel);
	if (word == "butterfly") return new Butterfly(rect, vel, game->getTexture(Game::BUTTERFLY), this);
	return nullptr;
}

void
PlayState::checkDefeat() {
	if (!juegoTerminado) {
		bool noArrows = bowPtr->getNumFlechas() <= 0 && arrowsInFlight <= 0;
		if (noArrows) {
			juegoPerdido = true;
			end();
		}
	}
}

void
PlayState::end() {
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

#pragma once
#include <stdexcept>
#include <string>
#include <SDL3/SDL.h>

// esto hereda directamente de logic_error sin añadir nada
class GameError : public std::logic_error
{
	public:
		GameError(const std::string& message) : std::logic_error(message) {}
};

// inicialización y SDL
class SDLError : public GameError
{
public:
	SDLError() : GameError(SDL_GetError()) {}
	SDLError(const std::string& message) : GameError(message + ": " + SDL_GetError()) {}
};

// fichero no encontrado
class FileNotFoundError : public GameError
{
private:
	std::string file;

public:
	FileNotFoundError(const std::string& file)
		: GameError("No se ha encontrado el archivo: " + file), file(file) {
	}

	const std::string& getFile() const { return file; }
};

// leer archivos (partida guardada, niveles)
class FileFormatError : public GameError
{
private:
	std::string file;
	int line;

public:
	FileFormatError(const std::string& file, int line, const std::string& message)
		: GameError(file + " (línea " + std::to_string(line) + "): " + message),
		file(file), line(line) {
	}

	const std::string& getFile() const { return file; }
	int getLine() const { return line; }
};

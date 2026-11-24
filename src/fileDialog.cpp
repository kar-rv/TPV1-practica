//
// Cuadro de diálogo reutilizable de apertura o guardado de archivo
//

#include "fileDialog.h"

#include <cstdlib>

#include "game.h"
#include "PlayState.h"
#include "GameObject.h"

FileDialog::FileDialog(PlayState* game, std::string basePath)
  : GameObject(game)
  , lastPath(basePath)
{
}

void
FileDialog::addFilter(const char* name, const char* pattern)
{
	filters.emplace_back(name, pattern);
}

void
FileDialog::openFile(Callback cb)
{
	callback = cb;

	SDL_ShowOpenFileDialog(&dialogCallback,
	                       this,
	                       game->getGame()->getWindow(),
	                       filters.data(),
	                       filters.size(),
	                       lastPath.empty() ? nullptr : lastPath.c_str(),
	                       false);
}

void
FileDialog::saveFile(Callback cb)
{
	callback = cb;

	SDL_ShowSaveFileDialog(&dialogCallback,
	                       this,
	                       game->getGame()->getWindow(),
	                       filters.data(),
	                       filters.size(),
	                       lastPath.empty() ? nullptr : lastPath.c_str());
}

void
FileDialog::dialogCallback(void* userdata, const char* const* fileList, int filter)
{
	// Esta función podría ejecutarse en un hilo distinto según la
	// documentación (y así ocurre en Windows), así que hay que evitar
	// modificar el estado aquí para evitar problemas de concurrencia
	// (TPV2). Se fija un booleano atómico y el callback se ejecuta en
	// el método update, en el hilo principal.

	FileDialog* self = static_cast<FileDialog*>(userdata);

	if (fileList != nullptr && *fileList != nullptr) {
		self->lastPath = *fileList;
		self->ready = true;
	}
}

void
FileDialog::render() const
{
	// No pinta nada
}

bool
FileDialog::update(float delta)
{
	// Ejecuta el callback si procede
	if (ready) {
		ready = false;
		callback(lastPath);
		callback = nullptr;
	}

	return true;
}

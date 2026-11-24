//
// Cuadro de diálogo reutilizable de apertura o guardado de archivo
//

#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <atomic>
#include <functional>
#include <string>
#include <vector>

#include <SDL3/SDL_dialog.h>

#include "gameObject.h"
#include "GameState.h"


/**
 * Cuadro de diálogo de apertura o guardado de archivo.
 */
class FileDialog : public GameObject
{
public:
	using Callback = std::function<void(std::string)>;

private:
	std::string lastPath;
	std::atomic<bool> ready;
	Callback callback;

	std::vector<SDL_DialogFileFilter> filters;

	static void dialogCallback(void* userdata, const char* const* fileList, int filter);

public:
	/**
	 * Crea un cuadro de diálogo con una ruta base en la que empezar.
	 */
	FileDialog(PlayState* game, std::string basePath = "");

	/**
	 * Añade un filtro de búsqueda al cuadro de diálogo.
	 */
	void addFilter(const char* name, const char* pattern);

	/**
	 * Abre el cuadro de diálogo para guardar un archivo y llama al callback con
	 * su nombre si la operación ha tenido éxito.
	 */
	void saveFile(Callback cb);

	/**
	 * Abre el cuadro de diálogo para abrir un archivo y llama al callback con
	 * su nombre si la operación ha tenido éxito.
	 */
	void openFile(Callback cb);

	// Métodos heredados de GameObject
	void render() const override;
	bool update(float delta) override;
};

#endif // FILE_DIALOG_H

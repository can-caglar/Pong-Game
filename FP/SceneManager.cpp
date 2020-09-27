#include "SceneManager.h"

SceneManager::SceneManager() {
	myRenderer = std::make_unique<Renderer>();
}
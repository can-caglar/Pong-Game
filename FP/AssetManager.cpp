#include <iostream>
#include "AssetManager.h"
#include "TextureManager.h"

SDL_Texture* AssetManager::getTexture(std::string t) {
	if (_textures.count(t)) {
		return _textures[t];
	}
	return nullptr;
}

TTF_Font* AssetManager::getFont(std::string t) {
	if (_fonts.count(t)) {
		return _fonts[t];
	}
	return nullptr;
}

void AssetManager::addTexture(std::string id, std::string filename) {
	_textures[id] = TextureManager::LoadTexture(filename.c_str());
}

void AssetManager::addFont(std::string id, std::string filename, int fontSize) {
	_fonts[id] = TTF_OpenFont(filename.c_str(), fontSize);
	if (_fonts[id] == NULL) {
		std::cout << "Failed to load " << filename << " << TTF_GetError() \n";
	}
}

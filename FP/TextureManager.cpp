#include "TextureManager.h"
#include <SDL_image.h>
#include <iostream>
#include "Game.h"

SDL_Texture* TextureManager::LoadTexture(const char* file) {

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(file);
	if (loadedSurface == NULL) {
		std::cout << "Unable to load image " << file << ". SDL_image error: " << IMG_GetError() << "\n";
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(Game::_renderer, loadedSurface);
		if (newTexture == NULL) {
			std::cout << "Unable to create texture from " << file << ". SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

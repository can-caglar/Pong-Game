#include "TextureManager.h"
#include <iostream>

TextureManager::~TextureManager() {
	for (SDL_Texture* r : _textures) {
		SDL_DestroyTexture(r);
	}

	for (auto f : _fonts) {
		TTF_CloseFont(f);
		f = NULL;
	}

}

TTF_Font* TextureManager::loadFont(std::string addr, int size) {

	auto font = TTF_OpenFont(addr.c_str(), size);
	if (font == nullptr) {
		std::cout << "Failed to load " << addr << " SDL_ttf error: " << TTF_GetError() << "\n";
	}
	return font;
}

SDL_Texture* TextureManager::loadTexture(std::string path, SDL_Renderer* _renderer) {
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cout << "Unable to load image " << path << ". SDL_image error: " << IMG_GetError() << "\n";
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
		if (newTexture == NULL) {
			std::cout << "Unable to create texture from " << path << ". SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}


SDL_Texture* TextureManager::loadTexture(std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* _renderer) {

	SDL_Texture* newTexture = NULL;

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, textureText.c_str(), textColor);
	if (textSurface == NULL) {
		std::cout << "Unable to render text surface. SDL_ttf error: " << TTF_GetError() << "\n";
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);
		if (newTexture == NULL) {
			std::cout << "Unable to create texture from rendered text. SDL_ttf error: " << TTF_GetError() << "\n";
		}
		else {
		}
		SDL_FreeSurface(textSurface);
	}
	return newTexture;
}

void TextureManager::addTexture(SDL_Texture* t) {
	_textures.emplace_back(t);
}

void TextureManager::addFont(TTF_Font* f) {
	_fonts.emplace_back(f);
}
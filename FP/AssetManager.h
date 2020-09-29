#pragma once

#include <SDL.h>
#include <unordered_map>
#include <string>
#include <SDL_ttf.h>

class AssetManager{

public:
	void addTexture(std::string id, std::string filename);
	void addFont(std::string id, std::string filename, int fontSize);

	SDL_Texture* getTexture(std::string t);
	TTF_Font* getFont(std::string t);

private:
	std::unordered_map<std::string, SDL_Texture*> _textures;
	std::unordered_map<std::string, TTF_Font*> _fonts;
};


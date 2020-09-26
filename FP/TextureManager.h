#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

class TextureManager
{
public:
	~TextureManager();
	// TODO add the other rule of 5 definitions

	SDL_Texture* loadTexture(std::string path, SDL_Renderer* _renderer);
	SDL_Texture* loadTexture(std::string textureTex,
										SDL_Color textColor, 
										TTF_Font* font,
										SDL_Renderer* _renderer);
	
	TTF_Font* loadFont(std::string addr, int size);

	void addTexture(SDL_Texture* t);
	void addFont(TTF_Font*);
	std::vector<SDL_Texture*> _textures;
	std::vector<TTF_Font*> _fonts;		// global font

private:
	

};


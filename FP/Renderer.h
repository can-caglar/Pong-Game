#pragma once

#include <memory>
#include <vector>
#include <SDL.h>

#include "Ball.h"
#include "Platform.h"

class Renderer
{
public:
	// Constructors
	Renderer();
	Renderer(Renderer& other) = delete;
	Renderer& operator=(Renderer& other) = delete;
	Renderer(Renderer&& other) = delete;
	Renderer& operator=(Renderer&& other) = delete;

	~Renderer();

	SDL_Window* _mainWindow;
	SDL_Renderer* _renderer;
	const int GAME_WIDTH = 600;
	const int GAME_HEIGHT = 400;

	SDL_Texture* loadTexture(std::string path);

	SDL_Texture* loadFromRenderedText(std::string textureTex, SDL_Color textColor, TTF_Font* font);

	bool loadMedia();

	void render();

	void addTexture(SDL_Texture* t);
	void renderText(SDL_Texture* text_texture, int xpos, int ypos);

	//void updateScoreTextTure();

	void init();

	std::unique_ptr<Ball> _ball;
	std::unique_ptr<Platform> _leftPlatform;
	std::unique_ptr<Platform> _rightPlatform;
	std::vector<TTF_Font*> _fonts;		// global font
private:
	std::vector<SDL_Texture*> _textures;

	




	enum _items {
		kBall,
		kLeftPlatform,
		kRightPlatform
	};
};


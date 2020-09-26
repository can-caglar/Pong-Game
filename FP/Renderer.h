#pragma once

#include <memory>
#include <vector>
#include <SDL.h>

#include "Ball.h"
#include "Platform.h"
#include "TextureManager.h"

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

	void init();

	std::unique_ptr<TextureManager> myTextureManager;

	bool loadMedia();

	void render();

	void renderThis(SDL_Texture* text_texture, int xpos, int ypos);

	//void updateScoreTextTure();

	std::unique_ptr<Ball> _ball;
	std::unique_ptr<Platform> _leftPlatform;
	std::unique_ptr<Platform> _rightPlatform;
private:

	enum _items {
		kBall,
		kLeftPlatform,
		kRightPlatform
	};
};


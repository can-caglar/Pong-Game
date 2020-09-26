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
	Renderer(SDL_Texture*, SDL_Texture*, SDL_Texture*);
	Renderer(Renderer& other) = delete;
	Renderer& operator=(Renderer& other) = delete;
	Renderer(Renderer&& other) = delete;
	Renderer& operator=(Renderer&& other) = delete;

	~Renderer();

	void render(SDL_Renderer* renderer);

	std::unique_ptr<Ball> _ball;
	std::unique_ptr<Platform> _leftPlatform;
	std::unique_ptr<Platform> _rightPlatform;

	const int GAME_WIDTH = 600;
	const int GAME_HEIGHT = 400;

	void addTexture(SDL_Texture* t);

private:
	std::vector<SDL_Texture*> _textures;

	enum _items {
		kBall,
		kLeftPlatform,
		kRightPlatform
	};
};


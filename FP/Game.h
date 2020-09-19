#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <SDL_ttf.h>

#include "GameObjects.h"
#include "CollisionDetection.h"

enum class GameState {
	kMainMenu,
	kStart,
};

class Game{

private:
	bool _running;
	int _frames;
	//std::vector<GameObjects>  //std::vector<SDL_Texture*> _textures;

	std::unique_ptr<Ball> _ball;
	std::unique_ptr<Platform> _leftPlatform;
	std::unique_ptr<Platform> _rightPlatform;

	SDL_Window* _mainWindow;
	SDL_Renderer* _renderer;
	SDL_Surface* _surface;

	const int _gameWidth = 600;
	const int _gameHeight = 400;

	TTF_Font* _font = NULL;		// global font

	SDL_Texture* loadTexture(std::string path);
	bool loadMedia();

	void checkCollisions();

	bool init();
	void onEvents(SDL_Event* event);
	void gameLoop();
	void render();
	void cleanUp();
	void start();
	
public:
	Game();
	int execute(); // Launch game

	
};


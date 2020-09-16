#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <string>

class Game{
private:
	bool _running;
	std::vector<SDL_Texture*> _textures;
	SDL_Window* _mainWindow;
	SDL_Texture* _currentTexture;
	SDL_Renderer* _renderer;

	SDL_Surface* _surface;

	const int _gameWidth = 600;
	const int _gameHeight = 400;

	int _testX = 0;
	int _testY = 0;

	SDL_Texture* loadTexture(std::string path);
	bool loadMedia();
	
public:
	Game();
	int execute(); // Launch game

	bool init();
	void onEvents(SDL_Event* event);
	void gameLoop();
	void render();
	void cleanUp();
};


#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Game{
private:
	bool _running;
public:
	Game();
	int execute(); // Launch game

	bool init();
	void events(SDL_Event event);
	void gameLoop();
	void render();
	void cleanUp();
};


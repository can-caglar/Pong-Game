#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include <chrono>
#include <mutex>
#include <future>

#include "GameObjects.h"
#include "CollisionDetection.h"
#include "Timer.h"


enum class GameState {
	kMainMenu,
	kPreStart,
	kStart,
	kScoreScreen,
};

class Game{

private:
	bool _running;
	int _frames;

	Timer _threadSafeTimer;

	std::vector<int> scores;

	std::unique_ptr<Ball> _ball;
	std::unique_ptr<Platform> _leftPlatform;
	std::unique_ptr<Platform> _rightPlatform;
	GameState _state;
	bool _gameStarted;

	SDL_Window* _mainWindow;
	SDL_Renderer* _renderer;

	const int GAME_WIDTH = 600;
	const int GAME_HEIGHT = 400;

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


#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include <SDL_ttf.h>

#include "GameObjects.h"
#include "Timer.h"

#include "Ball.h"
#include "Platform.h"
#include "AssetManager.h"
#include "Label.h"

class Timer;

enum class GameState {
	kMainMenu,
	kPreStart,
	kStart,
	kScoreScreen,
};

class Game{

public:
	Game();
	int execute(); // Launch game

	static SDL_Renderer* _renderer;

private:
	bool _running;
	int _frames;
	uint32_t _timeAtLaunch;

	Timer _threadSafeTimer;

	std::vector<int> _scoresVector;

	GameState _state;
	bool _gameStarted;

	SDL_Window* _mainWindow;
	
	int _game_width;
	int _game_height;

	std::unique_ptr<Ball> _ball;
	std::unique_ptr<Platform> _leftPlatform;
	std::unique_ptr<Platform> _rightPlatform;

	std::unique_ptr<Label>  _scoreLabel;
	std::unique_ptr<Label> _countdownTimer;
	std::unique_ptr<Label> _controlsLabel;

	AssetManager _assets;

	bool loadMedia();


	void checkAndReactToBallCollisions(int& winner);
	void checkAndReactToPlatformCollisions();

	bool init(std::string windowName, int x, int y, int width, int height, bool fullscreen);
	void onEvents(SDL_Event* event);
	void gameLoop();
	void render();
	void cleanUp();
};


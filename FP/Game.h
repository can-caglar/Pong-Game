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
#include <sstream>
#include <iomanip>

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
	uint32_t _timeAtLaunch;

	Timer _threadSafeTimer;

	std::vector<int> _scoresVector;

	GameState _state;
	bool _gameStarted;

	SDL_Window* _mainWindow;
	SDL_Renderer* _renderer;

	const int GAME_WIDTH = 600;
	const int GAME_HEIGHT = 400;

	std::unique_ptr<Ball> _ball;
	std::unique_ptr<Platform> _leftPlatform;
	std::unique_ptr<Platform> _rightPlatform;

	std::vector<TTF_Font*> _fonts;		// global font
	void renderText(SDL_Texture* text_texture, int xpos, int ypos);
	void updateScoreTextTure();

	SDL_Texture* _countdownTimer;
	SDL_Texture* _scoresTexture;
	SDL_Texture* _controlsTexture;

	SDL_Texture* loadTexture(std::string path);
	bool loadMedia();
	SDL_Texture* loadFromRenderedText(std::string textureTex, SDL_Color textColor, TTF_Font* font);

	void checkAndReactToBallCollisions(int& winner);
	void checkAndReactToPlatformCollisions();

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


#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include <SDL_ttf.h>

#include "GameObjects.h"

#include "Ball.h"
#include "Platform.h"
#include "Timer.h"

#include "Renderer.h"
#include "SceneManager.h"

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

private:
	bool _running;
	int _frames;
	uint32_t _timeAtLaunch;

	/*Timer _threadSafeTimer;*/

	std::vector<int> _scoresVector;

	GameState _state;
	bool _gameStarted;

	//std::unique_ptr<Renderer> myRenderer; //todo change name
	std::unique_ptr<SceneManager> mySceneManager;

	/*SDL_Texture* _countdownTimer;
	SDL_Texture* _scoresTexture;
	SDL_Texture* _controlsTexture;*/

	void checkAndReactToBallCollisions(int& winner);
	void checkAndReactToPlatformCollisions();

	bool init();
	void onEvents(SDL_Event* event);
	void gameLoop();
	void render();
	void cleanUp();
};


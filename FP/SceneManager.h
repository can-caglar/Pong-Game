#pragma once

#include <SDL.h>
#include <memory>
#include "Renderer.h"
#include "Timer.h"

class SceneManager
{
public:

	SceneManager();

	std::unique_ptr<Renderer> myRenderer; 

	SDL_Texture* _countdownTimer;
	SDL_Texture* _scoresTexture;
	SDL_Texture* _controlsTexture;
	Timer _threadSafeTimer;


};


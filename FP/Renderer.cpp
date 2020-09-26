#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Ball.h"
#include "Platform.h"
#include <iostream>
#include <exception>

Renderer::Renderer() {
	_ball = std::make_unique<Ball>(GAME_WIDTH / 2, GAME_HEIGHT / 2, 8);
	_leftPlatform = std::make_unique<Platform>(7, 150, 13, 73);
	_rightPlatform = std::make_unique<Platform>(580, 150, 13, 73);

	myTextureManager = std::make_unique<TextureManager>();

	init();
}

Renderer::~Renderer() {


	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_mainWindow);
	//SDL_FreeSurface(_gameSurface);

	_renderer = NULL;
	_mainWindow = NULL;

	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

void Renderer::init() {
	// Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		throw std::runtime_error(SDL_GetError());
	}

	// Create Window
	if ((_mainWindow = SDL_CreateWindow("Pong by Can", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_SHOWN)) == NULL) {
		throw std::runtime_error("SDL could not create window");
	}

	// Create renderer for window
	_renderer = SDL_CreateRenderer(_mainWindow, -1, SDL_RENDERER_ACCELERATED);
	if (_renderer == NULL) {
		throw std::runtime_error(SDL_GetError());
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		throw std::runtime_error(IMG_GetError());
	}

	// Initialize SDL TTF (text render)
	if (TTF_Init() == -1) {
		throw std::runtime_error(TTF_GetError());
	}

	// Load media
	if (loadMedia() == false) {
		throw std::runtime_error("Could not load all media!");
	}

}

//	Load textures from image and text
bool Renderer::loadMedia() {

	auto ball = myTextureManager->loadTexture("Resources/ball.png", _renderer);
	auto leftP = myTextureManager->loadTexture("Resources/plank.bmp", _renderer);
	auto rightP = myTextureManager->loadTexture("Resources/plank2.bmp", _renderer);

	// Has to be in this order
	myTextureManager->addTexture(ball);
	myTextureManager->addTexture(leftP);
	myTextureManager->addTexture(rightP);

	auto f1 = myTextureManager->loadFont("Resources/ARLRDBD.TTF", 28);
	myTextureManager->addFont(f1);

	auto f2 = myTextureManager->loadFont("Resources/ARLRDBD.TTF", 14);
	myTextureManager->addFont(f2);

	return true;
}


void Renderer::renderThis(SDL_Texture* tt, int xpos, int ypos) {

	if (tt == NULL) return;

	int w = 140;
	int h = 40;

	SDL_QueryTexture(tt, NULL, NULL, &w, &h);

	SDL_Rect newPos = { xpos, ypos , w, h };
	SDL_RenderCopy(_renderer, tt, NULL, &newPos);
}

void Renderer::render() {
	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0XFF, 0XFF);

	int width = 0;
	int height = 0;

	//TODO width/height query of texture doesn't have to happen every loop!

	//BALL
	SDL_QueryTexture(myTextureManager->_textures[kBall], NULL, NULL, &width, &height);
	SDL_Rect positionRect = { _ball->curX(), _ball->curY(), width, height };
	SDL_RenderCopy(_renderer, myTextureManager->_textures[kBall], NULL, &positionRect);

	// LEFT PLATFORM
	SDL_QueryTexture(myTextureManager->_textures[kLeftPlatform], NULL, NULL, &width, &height);
	positionRect = { _leftPlatform->curX(), _leftPlatform->curY(), width, height };
	SDL_RenderCopy(_renderer, myTextureManager->_textures[kLeftPlatform], NULL, &positionRect);

	// RIGHT PLATFORM
	SDL_QueryTexture(myTextureManager->_textures[kRightPlatform], NULL, NULL, &width, &height);
	positionRect = { _rightPlatform->curX(), _rightPlatform->curY(), width, height };
	SDL_RenderCopy(_renderer, myTextureManager->_textures[kRightPlatform], NULL, &positionRect);

	SDL_RenderPresent(_renderer);
}
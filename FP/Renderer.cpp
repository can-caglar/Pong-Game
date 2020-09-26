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

	init();
}

Renderer::~Renderer() {
	for (SDL_Texture* r : _textures) {
		SDL_DestroyTexture(r);
	}

	for (auto f : _fonts) {
		TTF_CloseFont(f);
		f = NULL;
	}

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_mainWindow);
	//SDL_FreeSurface(_gameSurface);

	_renderer = NULL;
	_mainWindow = NULL;

	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

void Renderer::addTexture(SDL_Texture* t) {
	_textures.emplace_back(t);
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

	auto ball = loadTexture("Resources/ball.png");
	auto leftP = loadTexture("Resources/plank.bmp");
	auto rightP = loadTexture("Resources/plank2.bmp");

	//myRenderer = std::make_unique<Renderer>(ball, leftP, rightP); // Create renderer
	// Has to be in this order
	addTexture(ball);
	addTexture(leftP);
	addTexture(rightP);

	_fonts.push_back(TTF_OpenFont("Resources/ARLRDBD.TTF", 28));
	if (_fonts[0] == NULL) {
		std::cout << "Failed to load 28 ARIAL ROUNDED font. SDL_ttf error: " << TTF_GetError() << "\n";
		return false;
	}

	_fonts.push_back(TTF_OpenFont("Resources/ARLRDBD.TTF", 14));
	if (_fonts[1] == NULL) {
		std::cout << "Failed to load 14 ARIAL ROUNDED font. SDL_ttf error: " << TTF_GetError() << "\n";
		return false;
	}

	return true;
}

SDL_Texture* Renderer::loadTexture(std::string path) {
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cout << "Unable to load image " << path << ". SDL_image error: " << IMG_GetError() << "\n";
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
		if (newTexture == NULL) {
			std::cout << "Unable to create texture from " << path << ". SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

SDL_Texture* Renderer::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font) {

	SDL_Texture* newTexture = NULL;

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, textureText.c_str(), textColor);
	if (textSurface == NULL) {
		std::cout << "Unable to render text surface. SDL_ttf error: " << TTF_GetError() << "\n";
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);
		if (newTexture == NULL) {
			std::cout << "Unable to create texture from rendered text. SDL_ttf error: " << TTF_GetError() << "\n";
		}
		else {
		}
		SDL_FreeSurface(textSurface);
	}
	return newTexture;
}

void Renderer::renderText(SDL_Texture* tt, int xpos, int ypos) {

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
	SDL_QueryTexture(_textures[kBall], NULL, NULL, &width, &height);
	SDL_Rect positionRect = { _ball->curX(), _ball->curY(), width, height };
	SDL_RenderCopy(_renderer, _textures[kBall], NULL, &positionRect);

	// LEFT PLATFORM
	SDL_QueryTexture(_textures[kLeftPlatform], NULL, NULL, &width, &height);
	positionRect = { _leftPlatform->curX(), _leftPlatform->curY(), width, height };
	SDL_RenderCopy(_renderer, _textures[kLeftPlatform], NULL, &positionRect);

	// RIGHT PLATFORM
	SDL_QueryTexture(_textures[kRightPlatform], NULL, NULL, &width, &height);
	positionRect = { _rightPlatform->curX(), _rightPlatform->curY(), width, height };
	SDL_RenderCopy(_renderer, _textures[kRightPlatform], NULL, &positionRect);

	SDL_RenderPresent(_renderer);
}
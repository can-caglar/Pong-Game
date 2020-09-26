#include <SDL.h>
#include "Renderer.h"
#include "Ball.h"
#include "Platform.h"

Renderer::Renderer() {
	_ball = std::make_unique<Ball>(GAME_WIDTH / 2, GAME_HEIGHT / 2, 8);
	_leftPlatform = std::make_unique<Platform>(7, 150, 13, 73);
	_rightPlatform = std::make_unique<Platform>(580, 150, 13, 73);
}
Renderer::Renderer(SDL_Texture* ball, SDL_Texture* lPlatform, SDL_Texture* rPlatform) {
	
	_ball = std::make_unique<Ball>(GAME_WIDTH / 2, GAME_HEIGHT / 2, 8);
	_leftPlatform = std::make_unique<Platform>(7, 150, 13, 73);
	_rightPlatform = std::make_unique<Platform>(580, 150, 13, 73);

	_textures.emplace_back(ball);
	_textures.emplace_back(lPlatform);
	_textures.emplace_back(rPlatform);
}

Renderer::~Renderer() {
	for (SDL_Texture* r : _textures) {
		SDL_DestroyTexture(r);
	}
}

void Renderer::addTexture(SDL_Texture* t) {
	_textures.emplace_back(t);
}

void Renderer::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0XFF, 0XFF);

	int width = 0;
	int height = 0;

	//TODO width/height query of texture doesn't have to happen every loop!

	//BALL
	SDL_QueryTexture(_textures[kBall], NULL, NULL, &width, &height);
	SDL_Rect positionRect = { _ball->curX(), _ball->curY(), width, height };
	SDL_RenderCopy(renderer, _textures[kBall], NULL, &positionRect);

	// LEFT PLATFORM
	SDL_QueryTexture(_textures[kLeftPlatform], NULL, NULL, &width, &height);
	positionRect = { _leftPlatform->curX(), _leftPlatform->curY(), width, height };
	SDL_RenderCopy(renderer, _textures[kLeftPlatform], NULL, &positionRect);

	// RIGHT PLATFORM
	SDL_QueryTexture(_textures[kRightPlatform], NULL, NULL, &width, &height);
	positionRect = { _rightPlatform->curX(), _rightPlatform->curY(), width, height };
	SDL_RenderCopy(renderer, _textures[kRightPlatform], NULL, &positionRect);

	SDL_RenderPresent(renderer);
}
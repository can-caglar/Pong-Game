#include "GameObjects.h"

Platform::Platform(const float& x, const float& y, const float& w, const float& h) {

	_curX = x;
	_curY = y;

	_boundingBox = SDL_Rect();
	_boundingBox.x = x;
	_boundingBox.y = y;
	_boundingBox.w = w;
	_boundingBox.h = h;
}

Ball::Ball(const float& x, const float& y, const float& r) {
	_curX = x;
	_curY = y;
	
	_boundingBox = Circle();
	_boundingBox.r = r;

	// Account for the fact that textures are drawn at top left, 
	// but circle x,y is in centre of circle.
	_boundingBox.x = x + r;
	_boundingBox.y = y + r;
	
}

void Ball::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0XFF, 0XFF);

	SDL_Rect newPos = { _curX, _curX, 32, 32 };
	SDL_RenderCopy(renderer, _texture, NULL, &newPos);

	//SDL_RenderPresent(renderer);
}

void Platform::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0XFF, 0XFF);

	SDL_Rect newPos = { _curX, _curY, 32, 32 };
	SDL_RenderCopy(renderer, _texture, NULL, &newPos);

	//SDL_RenderPresent(renderer);
}

void Platform::moveUp() {
	std::cout << "Platform moving up" << std::endl;
	setVelocity(Velocity{ 0, -1 });
}

void Platform::moveDown() {
	std::cout << "Platform moving down" << std::endl;
	setVelocity(Velocity{ 0, 1 });
}

void Platform::stop() {
	setVelocity(Velocity{ 0, 0 });
}

void GameObjects<Circle>::updateBoundingBox() {
	_boundingBox.x = _curX + _boundingBox.r;
	_boundingBox.y = _curY + _boundingBox.r;
}

void GameObjects<SDL_Rect>::updateBoundingBox() {
	_boundingBox.x = _curX;
	_boundingBox.y = _curY;
}


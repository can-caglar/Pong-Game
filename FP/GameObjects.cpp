#include "GameObjects.h"

Platform::Platform(const float& x, const float& y, const float& w, const float& h) {

	_velocity.x = 0;
	_velocity.y = 0;

	_curX = x;
	_curY = y;

	//_boundingBox = SDL_Rect();
	_boundingBox.x = x;
	_boundingBox.y = y;
	_boundingBox.w = w;
	_boundingBox.h = h;
}

Ball::Ball(const float& x, const float& y, const float& r) {

	_mt.seed(_rdevice());
	std::uniform_int_distribution<int> dist(0, 1);

	if (dist(_mt) == 1)
		_velocity.x = 2;
	else
		_velocity.x = -2;
	
	std::uniform_int_distribution<int> dist2(1, 3);
	_velocity.y = static_cast<float>(dist2(_mt));

	//_velocity.x = -2; // TODO: REMOVE THIS LINE
	//_velocity.y = 0; // TODO: REMOVE THIS LINE

	_curX = x;
	_curY = y;
	
	//_boundingBox = Circle();
	_boundingBox.r = r;

	// Account for the fact that textures are drawn at top left, 
	// but circle x,y is in centre of circle.
	_boundingBox.x = x + r;
	_boundingBox.y = y + r;
	
}

void Ball::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0XFF, 0XFF);

	SDL_Rect newPos = { _curX, _curY, 15, 15 };
	SDL_RenderCopy(renderer, _texture, NULL, &newPos);

	//SDL_RenderPresent(renderer);
}

void Platform::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0XFF, 0XFF);

	SDL_Rect newPos = { _curX, _curY, 13, 73 };
	SDL_RenderCopy(renderer, _texture, NULL, &newPos);

	//SDL_RenderPresent(renderer);
}

void Platform::moveUp() {
	std::cout << "Platform moving up" << std::endl;
	setVelocity(Velocity{ 0, -3 });
}

void Platform::moveDown() {
	std::cout << "Platform moving down" << std::endl;
	setVelocity(Velocity{ 0, 3 });
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


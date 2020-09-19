#include "GameObjects.h"

Platform::Platform(const float& x, const float& y, const float& w, const float& h) {

	_velocity.x = 0;
	_velocity.y = 0;

	setSpeed(4);

	_curX = x;
	_curY = y;

	//_boundingBox = SDL_Rect();
	_boundingBox.x = x;
	_boundingBox.y = y;
	_boundingBox.w = w;
	_boundingBox.h = h;
}

Ball::Ball(const float& x, const float& y, const float& r) {

	setSpeed(3);

	//setRandomVelocity();

	_curX = x;
	_curY = y;

	_boundingBox.r = r;

	// Account for the fact that textures are drawn at top left, 
	// but circle x,y is in centre of circle.
	_boundingBox.x = x + r;
	_boundingBox.y = y + r;
	
}

// Sets a random y velocity going towards left of right. left/right speed remains the same as before.
void Ball::setRandomVelocity() {
	_mt.seed(_rdevice());
	std::uniform_int_distribution<int> dist(0, 1);

	if (dist(_mt) == 1)
		_velocity.x = _speed;
	else
		_velocity.x = -_speed;

	std::uniform_int_distribution<int> dist2(1, 3);
	_velocity.y = static_cast<float>(dist2(_mt));
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
	setVelocity(Vect_2D{ 0, -_speed });
}

void Platform::moveDown() {
	setVelocity(Vect_2D{ 0, _speed });
}

void Platform::stop() {
	setVelocity(Vect_2D{ 0, 0 });
}

void GameObjects<Circle>::updateBoundingBox() {
	_boundingBox.x = _curX + _boundingBox.r;
	_boundingBox.y = _curY + _boundingBox.r;
}

void GameObjects<SDL_Rect>::updateBoundingBox() {
	_boundingBox.x = _curX;
	_boundingBox.y = _curY;
}

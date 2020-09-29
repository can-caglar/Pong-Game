#include "Ball.h"
#include <iostream>

Ball::Ball(SDL_Texture* texture, const int& x, const int& y) {

	int width;
	int height;
	int r;

	setSpeed(8);

	_texture = texture;
	_curX = x;
	_curY = y;

	width = 0;
	height = 0;
	r = 0;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	r = std::max(width, height);

	_boundingBox.r = r;

	//Textures are drawn at top left, but bounding box is drawn from center.
	_boundingBox.x = x + r;
	_boundingBox.y = y + r;

}

// Sets a random y velocity going towards left of right. left/right speed remains the same as before.
void Ball::setRandomVelocity() {
	int a = _rdevice();
	std::cout << "random seed = " << a << "\n";
	_mt.seed(a);
	std::uniform_int_distribution<int> dist(0, 1);

	if (dist(_mt) == 1)
		_velocity.x = _speed;
	else
		_velocity.x = -_speed;

	std::uniform_int_distribution<int> dist2(-3, 3);
	_velocity.y = dist2(_mt);

}

void Ball::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0XFF, 0XFF);

	SDL_Rect newPos = { _curX, _curY, _boundingBox.r*2, _boundingBox.r*2 };
	SDL_RenderCopy(renderer, _texture, NULL, &newPos);

	//SDL_RenderPresent(renderer);
}
#include <iostream>

#include "Ball.h"
#include "Shapes.h"

Ball::Ball(const int& x, const int& y, const int& r) {

	setSpeed(8);

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

	SDL_Rect newPos = { _curX, _curY, 15, 15 };
	SDL_RenderCopy(renderer, _texture, NULL, &newPos);

	//SDL_RenderPresent(renderer);
}
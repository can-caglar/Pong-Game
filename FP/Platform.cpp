#include "Platform.h"

Platform::Platform(const int& x, const int& y, const int& w, const int& h) {

	_velocity.x = 0;
	_velocity.y = 0;

	setSpeed(7);

	_curX = x;
	_curY = y;

	//_boundingBox = SDL_Rect();
	_boundingBox.x = x;
	_boundingBox.y = y;
	_boundingBox.w = w;
	_boundingBox.h = h;
}


//void Platform::render(SDL_Renderer* renderer) {
//	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0XFF, 0XFF);
//
//	SDL_Rect newPos = { _curX, _curY, 13, 73 };
//	SDL_RenderCopy(renderer, _texture, NULL, &newPos);
//
//	//SDL_RenderPresent(renderer);
//}

void Platform::moveUp() {
	setVelocity(Vect_2D{ 0, -_speed });
}

void Platform::moveDown() {
	setVelocity(Vect_2D{ 0, _speed });
}

void Platform::stop() {
	setVelocity(Vect_2D{ 0, 0 });
}
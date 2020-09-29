#include "Platform.h"

Platform::Platform(SDL_Texture* texture, const int& x, const int& y) {

	_texture = texture;

	_velocity.x = 0;
	_velocity.y = 0;

	setSpeed(7);

	_curX = x;
	_curY = y;

	int width = 0;
	int height = 0;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	
	_boundingBox.x = x;
	_boundingBox.y = y;
	_boundingBox.w = width;
	_boundingBox.h = height;
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
#include <iostream>
#include "GameObjects.h"

template<>
void GameObjects<Circle>::updateBoundingBox() {
	_boundingBox.x = _curX + _boundingBox.r;
	_boundingBox.y = _curY + _boundingBox.r;
}

template<>
void GameObjects<SDL_Rect>::updateBoundingBox() {
	_boundingBox.x = _curX;
	_boundingBox.y = _curY;
}


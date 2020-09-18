#include "GameObjects.h"

Ball::Ball(const float& x, const float& y, const Circle& circle) {
	_curX = x;
	_curY = y;
	
	_boundingBox = circle;

	// Account for the fact that textures are drawn at top left, 
	// but circle x,y is in centre of circle.
	_boundingBox.x = x + circle.r;
	_boundingBox.y = y + circle.r;
	
}

void Ball::move() {
	std::cout << "Ball bounced back!" << std::endl;
}


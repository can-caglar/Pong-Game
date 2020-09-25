#include "CollisionDetection.h"
#include <iostream>
#include <cmath>

// Checks if circle and rectangle have collided. Returns 2 ints representing where on x and y they collided. Both will be -1, -1 if no collision.
void CollisionDetection::detectCollision(const Circle& circle, const SDL_Rect& rectangle, int& collision_x, int& collision_y) {

	collision_x = -1;
	collision_y = -1;

	int rectCollidePointY = 0;
	int rectCollidePointX = 0;

	// Check where on the y axis the circle is in relation to the rectangle
	if (circle.y > rectangle.y + rectangle.h)  rectCollidePointY = rectangle.y + rectangle.h;	// circle below rectangle
	else if (circle.y < rectangle.y) rectCollidePointY = rectangle.y;							// circle above rectangle
	else rectCollidePointY = circle.y;															// circle somewhere in the middle of rectangle in y axis

	// Check where on the x axis the circle is in relation to the rectangle
	if (circle.x > rectangle.x + rectangle.w)  rectCollidePointX = rectangle.x + rectangle.w;	// circle to the right of whole rectangle
	else if (circle.x < rectangle.x) rectCollidePointX = rectangle.x;							// circle to the left of whole rectangle
	else rectCollidePointX = circle.x;															// circle somewhere in the middle of rectangle in x axis

	int d = square_of_distance(circle.x, circle.y, rectCollidePointX, rectCollidePointY);

	if (d < pow(circle.r, 2)) {
		collision_x = rectCollidePointX;
		collision_y = rectCollidePointY;
		return;
	}
}

int CollisionDetection::square_of_distance(int x1, int y1, int x2, int y2) {
	return static_cast<int>(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

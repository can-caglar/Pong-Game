#include "CollisionDetection.h"
#include <iostream>

// Checks if circle and rectangle have collided. Returns 2 floats representing where on x and y they collided. Both will be -1, -1 if no collision.
void CollisionDetection::detectCollision(const Circle& circle, const SDL_Rect& rectangle, float& collision_x, float& collision_y) {

	collision_x = -1;
	collision_y = -1;

	float rect_y_to_observe = 0;
	float rect_x_to_observe = 0;

	// Check where on the y axis the circle is in relation to the rectangle
	if (circle.y > rectangle.y + rectangle.h)  rect_y_to_observe = rectangle.y + rectangle.h;	// circle below rectangle
	else if (circle.y < rectangle.y) rect_y_to_observe = rectangle.y;							// circle above rectangle
	else rect_y_to_observe = circle.y;															// circle somewhere in the middle of rectangle

	// If circle is to the right of rectangle
	if (circle.x > rectangle.x + rectangle.w) {
		rect_x_to_observe = rectangle.x + rectangle.w;

		float d = square_of_distance(circle.x, circle.y, rect_x_to_observe, rect_y_to_observe);
		if (d < circle.r) {
			//COLLISION
			collision_x = rect_x_to_observe;
			collision_y = rect_y_to_observe;
			return;
		}
	}
	// If circle is to the left of platform
	if (circle.x < rectangle.x) {
		rect_x_to_observe = rectangle.x;

		float d = square_of_distance(circle.x, circle.y, rect_x_to_observe, rect_y_to_observe);
		if (d < circle.r) {
			//COLLLISION
			collision_x = rect_x_to_observe;
			collision_y = rect_y_to_observe;
			return;
		}
	}
	// If circle is above platform
	if (circle.y > rectangle.y) {
		int a = 1;
	}

	// If circle is below platform 2
	if (circle.y < rectangle.y + rectangle.h) {
		int a = 1;
	}

}

float CollisionDetection::square_of_distance(float x1, float y1, float x2, float y2) {
	return powf(x1 - x2, 2) + powf(y1 - y2, 2);
}

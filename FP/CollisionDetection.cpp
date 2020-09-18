#include "CollisionDetection.h"
#include <iostream>

bool CollisionDetection::detectCollision(const Circle& circle, const SDL_Rect& rectangle) {

	bool collision = false;
	// Platform boundaries

	std::cout << "circle x: " << circle.x << " y: " << circle.y << " r: " << circle.r << "\t";
	std::cout << "rectangle left x: " << rectangle.x << " y: " << rectangle.y << " rectangle right x: " << rectangle.x + rectangle.w << " down y: " << rectangle.y + rectangle.h<< "\t";

	// If circle is to the right of rectangle
	if (circle.x > rectangle.x + rectangle.w) {
		std::cout << "circle to the right: \n";
		float rect_x_to_observe = rectangle.x + rectangle.w;
		float rect_y_to_observe = 0;
		if (circle.y > rectangle.y + rectangle.h)  rect_y_to_observe = rectangle.y + rectangle.h;	// circle below rectangle
		else if (circle.y < rectangle.y) rect_y_to_observe = rectangle.y;							// circle above rectangle
		else rect_y_to_observe = circle.y;															// circle somewhere in the middle of rectangle

		float d = square_of_distance(circle.x, circle.y, rect_x_to_observe, rect_y_to_observe);
		if (d < circle.r) {
			collision = true;
		}
	}
	// If circle is to the left of platform
	if (circle.x < rectangle.x) {
		std::cout << "circle to the left: \n";
		float rect_x_to_observe = rectangle.x;
		float rect_y_to_observe = 0;
		if (circle.y > rectangle.y + rectangle.h)  rect_y_to_observe = rectangle.y + rectangle.h;	// circle below rectangle
		else if (circle.y < rectangle.y) rect_y_to_observe = rectangle.y;							// circle above rectangle
		else rect_y_to_observe = circle.y;															// circle somewhere in the middle of rectangle

		float d = square_of_distance(circle.x, circle.y, rect_x_to_observe, rect_y_to_observe);
		if (d < circle.r) {
			collision = true;
		}
	}
	// If circle is above platform
	if (circle.x > rectangle.x) {

	}

	// If circle is below platform 2
	if (circle.x < rectangle.y + rectangle.h) {

	}

	return collision;
}

float CollisionDetection::square_of_distance(float x1, float y1, float x2, float y2) {
	return powf(x1 - x2, 2) + powf(y1 - y2, 2);
}

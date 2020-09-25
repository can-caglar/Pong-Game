#pragma once

struct Circle;
struct SDL_Rect;

class CollisionDetection
{
public:
	static int square_of_distance(int x1, int y1, int x2, int y2);
	static void detectCollision(const Circle& item1, const SDL_Rect& item2, int& collisionX, int& collisionY);

};


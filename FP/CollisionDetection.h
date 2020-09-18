#pragma once
#include <cmath>
#include "GameObjects.h"

class CollisionDetection
{
public:
	static float square_of_distance(float x1, float y1, float x2, float y2);
	static bool detectCollision(const Circle& item1, const SDL_Rect& item2);
	//static bool detectCollision(const SDL_Rect& item1, const SDL_Rect& item2);
	//static bool detectCollision(const Circle& item1, );
};


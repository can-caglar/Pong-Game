#pragma once
#include "GameObjects.h"

class CollisionDetection
{
public:
	static bool detectCollision(const Circle& item1, const SDL_Rect& item2);
	static bool detectCollision(const SDL_Rect& item1, const SDL_Rect& item2);
	//static bool detectCollision(const Circle& item1, );
};


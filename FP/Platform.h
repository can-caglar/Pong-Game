#pragma once
#include "GameObjects.h"

class Platform : public GameObjects<SDL_Rect> {
public:
	Platform() = default;
	Platform(const int& x, const int& y, const int& w, const int& h);

	// Functions
	void moveUp();
	void moveDown();
	void stop();
	//void render(SDL_Renderer* renderer) override;
};

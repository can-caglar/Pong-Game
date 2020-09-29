#pragma once
#include "GameObjects.h"

class Platform : public GameObjects<SDL_Rect> {
public:
	Platform() = default;
	Platform(SDL_Texture* texture,  const int& x, const int& y);

	// Functions
	void moveUp();
	void moveDown();
	void stop();
	void render(SDL_Renderer* renderer) override;
};

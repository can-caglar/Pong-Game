#pragma once
#include "GameObjects.h"

class Ball : public GameObjects<Circle> {

public:
	Ball() = default;
	Ball(SDL_Texture* texture, const int& x, const int& y);

	// Functions
	void setRandomVelocity();
	void render(SDL_Renderer* renderer) override;

private:
	std::mt19937 _mt;
	std::random_device _rdevice;

};

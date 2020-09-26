#pragma once
#include "GameObjects.h"

struct Circle;

class Ball : public GameObjects<Circle> {

public:
	Ball() = default;
	Ball(const int& x, const int& y, const int& r);

	// Functions
	void setRandomVelocity();
	void render(SDL_Renderer* renderer) override;

private:
	std::mt19937 _mt;
	std::random_device _rdevice;

};

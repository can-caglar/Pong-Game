#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <iostream>

struct Velocity {

	float x;
	float y;
};

struct Circle {

	float x;
	float y;
	float r;
};

template <typename T>
class GameObjects{

protected:
	Velocity _velocity;
	float _curX;
	float _curY;
	SDL_Texture* _texture;
	T _boundingBox;

public:
	// Constructors
	GameObjects() : _velocity{ 0,0 }, _curX(0), _curY(0) {};

	// Accessors
	Velocity velocity() { return _velocity; }
	float curX() { return _curX; }
	float curY() { return _curY; }
	T boundingBox() { return _boundingBox;  }

	// Modifiers
	virtual void move() = 0;
	
};

class Ball : public GameObjects<Circle> {

public:
	Ball() = default;
	Ball(const float& x, const float& y, const Circle& circle);

	// Functions
	void move() override;
};

class Platform : public GameObjects<SDL_Rect> {

};
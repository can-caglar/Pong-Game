#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <iostream>
#include <random>

struct Vect_2D {

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

	Vect_2D _velocity;
	float _speed;

	float _curX;
	float _curY;
	SDL_Texture* _texture;	// TODO need to destroy textures // SDL_DestroyTexture(t);
	T _boundingBox;

public:

	// Accessors
	Vect_2D velocity() { return _velocity; }
	float curX() { return _curX; }
	float curY() { return _curY; }
	T boundingBox() { return _boundingBox;  }
	SDL_Texture* getTexture() { return _texture;  }

	// Modifiers
	void setTexture(SDL_Texture* t) { _texture = t; }
	void setVelocity(Vect_2D v) { _velocity = v;  }
	void setSpeed(float s) { _speed = s;  }
	void curX(float n); 
	void curY(float n); 
	
	// Special functions
	virtual void render(SDL_Renderer* renderer) = 0;
	void move();
	void updateBoundingBox();
};

class Ball : public GameObjects<Circle> {

private:
	std::mt19937 _mt;
	std::random_device _rdevice;
public:

	Ball() = default;
	Ball(const float& x, const float& y, const float& r);

	// Functions
	void render(SDL_Renderer* renderer) override;
	void setRandomVelocity();
};

class Platform : public GameObjects<SDL_Rect> {

public:

	Platform() = default;
	Platform(const float& x, const float& y, const float& w, const float& h);

	// Functions
	void moveUp();
	void moveDown();
	void stop();
	void render(SDL_Renderer * renderer) override;
};

template <typename T>
void GameObjects<T>::move() {
	_curX += _velocity.x;
	_curY += _velocity.y;
	updateBoundingBox();
}

template <typename T>
void GameObjects<T>::curX(float n) {
	_curX = n;
	updateBoundingBox();
}

template <typename T>
void GameObjects<T>::curY(float n) {
	_curY = n;
	updateBoundingBox();
}

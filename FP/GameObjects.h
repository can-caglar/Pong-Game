#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <random>

#include "Shapes.h"

template <typename T>
class GameObjects{

public:

	// Accessors
	Vect_2D velocity() { return _velocity; }
	int curX() { return _curX; }
	int curY() { return _curY; }
	T boundingBox() { return _boundingBox; }
	
	// Modifiers
	//void setTexture(SDL_Texture * t) { _texture = t; }
	void setVelocity(Vect_2D v) { _velocity = v; }
	void setSpeed(int s) { _speed = s; }
	void curX(int n);
	void curY(int n);

	// Special functions
	void move();
	void updateBoundingBox();

protected:
	Vect_2D _velocity;
	int _speed;

	int _curX;
	int _curY;
	T _boundingBox;

};


template <typename T>
void GameObjects<T>::move() {
	_curX += _velocity.x;
	_curY += _velocity.y;
	updateBoundingBox();
}

template <typename T>
void GameObjects<T>::curX(int n) {
	_curX = n;
	updateBoundingBox();
}

template <typename T>
void GameObjects<T>::curY(int n) {
	_curY = n;
	updateBoundingBox();
}

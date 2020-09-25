#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <random>

struct Vect_2D {

	int x;
	int y;
};

struct Circle {

	int x;
	int y;
	int r;
};

template <typename T>
class GameObjects{

public:
	// Constructors
	GameObjects() = default;
	GameObjects(GameObjects & other) = delete;
	GameObjects& operator=(GameObjects & other) = delete;
	GameObjects(GameObjects && other) = delete;
	GameObjects& operator=(GameObjects && other) = delete;

	// Destructor
	~GameObjects() { SDL_DestroyTexture(_texture); }

	// Accessors
	Vect_2D velocity() { return _velocity; }
	int curX() { return _curX; }
	int curY() { return _curY; }
	T boundingBox() { return _boundingBox; }
	SDL_Texture* getTexture() { return _texture; }

	// Modifiers
	void setTexture(SDL_Texture * t) { _texture = t; }
	void setVelocity(Vect_2D v) { _velocity = v; }
	void setSpeed(int s) { _speed = s; }
	void curX(int n);
	void curY(int n);

	// Special functions
	virtual void render(SDL_Renderer * renderer) = 0;
	void move();
	void updateBoundingBox();

protected:
	Vect_2D _velocity;
	int _speed;

	int _curX;
	int _curY;
	SDL_Texture* _texture;	// TODO need to destroy textures // SDL_DestroyTexture(t);
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

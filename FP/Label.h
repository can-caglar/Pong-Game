#pragma once
#include <SDL_ttf.h>

class Label
{

public:
	Label(std::string text, std::string fontAddr, int x, int y, int size, SDL_Color color) :
		_text(text), _fontAddress(fontAddr), _fontSize(size), _colour(color)
	{
		_posRect.x = x;
		_posRect.y = y;
		_texture = nullptr;
		setText();
	};

	void changeText(std::string);
	void show(int x = -1, int y = -1);

	SDL_Rect getRect() { return _posRect;  }

private:
	void setText();
	
	SDL_Rect _posRect;

	std::string _text;
	int _fontSize;
	SDL_Color _colour;
	std::string _fontAddress;
	SDL_Texture* _texture;
};


#include <iostream>

#include "Label.h"
#include "Game.h"

void Label::changeText(std::string text) {
	_text = text;
	setText();
}

void Label::show(int x, int y) {

	if (x != -1) _posRect.x = x;
	if (y != -1) _posRect.y = y;

	SDL_RenderCopy(Game::_renderer, _texture, NULL, &_posRect);
}

void Label::setText() {
	SDL_Texture* newTexture = NULL;

	TTF_Font* font = TTF_OpenFont(_fontAddress.c_str(), _fontSize);

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, _text.c_str(), _colour);
	if (textSurface == NULL) {
		std::cout << "Unable to render text surface. SDL_ttf error: " << TTF_GetError() << "\n";
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(Game::_renderer, textSurface);
		if (newTexture == NULL) {
			std::cout << "Unable to create texture from rendered text. SDL_ttf error: " << TTF_GetError() << "\n";
		}
		else {
		}
		SDL_FreeSurface(textSurface);
	}
	
	if (_texture != nullptr) {
		SDL_DestroyTexture(_texture);
	}

	_texture = newTexture;

	SDL_QueryTexture(_texture, NULL, NULL, &_posRect.w, &_posRect.h);
}
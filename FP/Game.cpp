#include "Game.h"

using std::cout;
using std::endl;

enum class Textures {
	ball = 0,
	total_textures,
};

Game::Game() {
	cout << "Game object initialized." << endl;

	_textures = std::vector<SDL_Texture*>(static_cast<int>(Textures::total_textures));
	_running = true;

	_testX = _gameWidth / 2 - 32;
	_testY = _gameHeight / 2 - 32;
}

int Game::execute() {
	cout << "Launching game." << endl;

	init();

	SDL_Event e;
	cout << "Starting game..." << endl;
	while (_running) {
		while (SDL_PollEvent(&e)) {
			onEvents(&e);
		}
		gameLoop();
		render();
	}

	cleanUp();
	return 0;
}

bool Game::init() {
	cout << "Initializing game." << "\n" << std::flush;

	// Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "SDL couldn't initialize! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	// Create Window
	if ((_mainWindow = SDL_CreateWindow("Pong by Can", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_gameWidth, _gameHeight, SDL_WINDOW_SHOWN)) == NULL) {
		return false;
	}

	// Create renderer for window
	_renderer = SDL_CreateRenderer(_mainWindow, -1, SDL_RENDERER_ACCELERATED);
	if (_renderer == NULL) {
		std::cout << "Renderer could not be created.  SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
		return false;
	}

	// Initialize surface
	_surface = SDL_GetWindowSurface(_mainWindow);

	// Load media
	if (loadMedia() == false) {
		return false;
	}
	
	return true;
}

bool Game::loadMedia() {
	
	_textures[0] = loadTexture("Resources/ball.bmp");
	return true;
}

SDL_Texture* Game::loadTexture(std::string path) {
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cout << "Unable to load image " << path << ". SDL_image error: " << IMG_GetError() << "\n";
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
		if (newTexture == NULL) {
			std::cout << "Unable to create texture from " << path << ". SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

void Game::onEvents(SDL_Event* event) {
	if (event->type == SDL_QUIT) {
		_running = false;
	}
	else if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym)
		{
		case SDLK_UP:
			_currentTexture = _textures[0];
			_testY = (_testY - 10) % _gameHeight;
			break;
		case SDLK_DOWN:
			_currentTexture = _textures[0];
			_testY = (_testY + 10) % _gameHeight;
			break;
		case SDLK_LEFT:
			_currentTexture = _textures[0];
			_testX = (_testX - 10) % _gameWidth;
			break;
		case SDLK_RIGHT:
			_currentTexture = _textures[0];
			_testX = (_testX + 10) % _gameWidth;
			break;
		default:
			break;
		}
	}
}

void Game::gameLoop() {

	SDL_Delay(1);
}

void Game::render() {
	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0XFF, 0XFF);
	SDL_RenderClear(_renderer);

	//SDL_Rect viewPort{ 0,0, screen_width / 2, screen_height / 2 };
	//SDL_RenderSetViewport(_renderer, &viewPort);
	
	_currentTexture = _textures[0];
	SDL_Rect newPos = { _testX, _testY, 32, 32 };
	SDL_RenderCopy(_renderer, _currentTexture, NULL, &newPos);
	//SDL_SetRenderFillRect(renderer, rect)

	// SDL_GetTicks(); //returns time in ms since sdl init. SDL_Delay() // DELAYS
	SDL_RenderPresent(_renderer);
}

void Game::cleanUp() {
	cout << "End. Cleaning up..." << endl;

	for (auto t : _textures) {
		SDL_DestroyTexture(t);
		t = NULL;
	}

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_mainWindow);
	//SDL_FreeSurface(_gameSurface);

	IMG_Quit();
	SDL_Quit();

}




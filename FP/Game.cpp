#include "Game.h"

using std::cout;
using std::endl;


Game::Game() {
	cout << "Game object initialized." << endl;

	_ball = std::make_unique<Ball>(_gameWidth / 2, _gameHeight / 2, 32);
	_leftPlatform = std::make_unique<Platform>(200, 200, 32, 32);
	_rightPlatform = std::make_unique<Platform>(400, 200, 32, 32);
	_running = true;
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
	
	_ball->setTexture(loadTexture("Resources/ball.bmp"));
	_leftPlatform->setTexture(loadTexture("Resources/plank.bmp"));
	_rightPlatform->setTexture(loadTexture("Resources/plank2.bmp"));

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
			_leftPlatform->moveUp();
			break;
		case SDLK_DOWN:
			_leftPlatform->moveDown();
			break;
		case SDLK_LEFT:

			break;
		case SDLK_RIGHT:

			break;
		default:
			break;
		}
	}
}

void Game::gameLoop() {

	_leftPlatform->move();
	if (_leftPlatform->curX() < 0) {
		_leftPlatform->curX(0);
		_leftPlatform->stop();
	}
	if (_leftPlatform->curX() > _gameWidth) {
		_leftPlatform->curX(_gameWidth);
		_leftPlatform->stop();
	}
	if (_leftPlatform->curY() < 0) {
		_leftPlatform->curY(0);
		_leftPlatform->stop();
	}
	if (_leftPlatform->curY() > _gameHeight) {
		_leftPlatform->curY(_gameHeight);
		_leftPlatform->stop();
	}

	_rightPlatform->move();
	_ball->move();
	
}

void Game::render() {

	
	_leftPlatform->render(_renderer);
	_ball->render(_renderer);

	SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0x00);
	//SDL_RenderClear(_renderer);

	////SDL_Rect viewPort{ 0,0, screen_width / 2, screen_height / 2 };
	////SDL_RenderSetViewport(_renderer, &viewPort);
	//
	//_currentTexture = _textures[0];
	//SDL_Rect newPos = { _testX, _testY, 32, 32 };
	//SDL_RenderCopy(_renderer, _currentTexture, NULL, &newPos);
	////SDL_SetRenderFillRect(renderer, rect)

	//// SDL_GetTicks(); //returns time in ms since sdl init. SDL_Delay() // DELAYS
	SDL_RenderPresent(_renderer);
}

void Game::cleanUp() {
	cout << "End. Cleaning up..." << endl;

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_mainWindow);
	//SDL_FreeSurface(_gameSurface);

	IMG_Quit();
	SDL_Quit();

}




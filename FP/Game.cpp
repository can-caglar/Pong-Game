#include "Game.h"
#include "CollisionDetection.h"

using std::cout;
using std::endl;


Game::Game() {
	cout << "Game object initialized." << endl;

	_ball = std::make_unique<Ball>(_gameWidth / 2, _gameHeight / 2, 7.5f);
	_leftPlatform = std::make_unique<Platform>(20, 150, 13, 73);
	_rightPlatform = std::make_unique<Platform>(580, 150, 13, 73);
	_running = true;
	_frames = 0;
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
	
	_ball->setTexture(loadTexture("Resources/ball.png"));
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
			_rightPlatform->moveUp();
			break;
		case SDLK_DOWN:
			_rightPlatform->moveDown();
			break;
		case SDLK_w:
			_leftPlatform->moveUp();
			break;
		case SDLK_s:
			_leftPlatform->moveDown();
			break;
		default:
			break;
		}
	}
	else if (event->type == SDL_KEYUP) {
		switch (event->key.keysym.sym)
		{
		case SDLK_UP:
			_rightPlatform->stop();
			break;
		case SDLK_DOWN:
			_rightPlatform->stop();
			break;
		case SDLK_w:
			_leftPlatform->stop();
			break;
		case SDLK_s:
			_leftPlatform->stop();
			break;
		default:
			break;
		}
	}
}

void Game::checkCollisions() {

	// Left platform
	if (_leftPlatform->curY() < 0) {
		_leftPlatform->curY(0);
		_leftPlatform->stop();
	}
	if ((_leftPlatform->curY() + _leftPlatform->boundingBox().h) > (_gameHeight)) {
		_leftPlatform->curY(_gameHeight - _leftPlatform->boundingBox().h);
		_leftPlatform->stop();
	}

	// Right platform
	if (_rightPlatform->curY() < 0) {
		_rightPlatform->curY(0);
		_rightPlatform->stop();
	}

	if ((_rightPlatform->curY() + _rightPlatform->boundingBox().h) > (_gameHeight)) {
		_rightPlatform->curY(_gameHeight - _rightPlatform->boundingBox().h);
		_rightPlatform->stop();
	}

	// Ball
	float ballDiameter = 2 * _ball->boundingBox().r;
	if (_ball->curX() < 0) {
		// PLAYER 2 WINS
		_ball->setVelocity({ 0, 0 });
	}
	if (_ball->curX() > _gameWidth - ballDiameter) {
		// PLAYER 1 WINS
		_ball->setVelocity({ 0, 0 });
	}
	if (_ball->curY() < 0) {
		float yVel = _ball->velocity().y;
		float xVel = _ball->velocity().x;
		float curY = _ball->curY();

		_ball->setVelocity({ xVel, -yVel });
		_ball->curY(0);
	}
	if (_ball->curY() > _gameHeight - ballDiameter) {

		float yVel = _ball->velocity().y;
		float xVel = _ball->velocity().x;
		float curY = _ball->curY();

		_ball->setVelocity({ xVel, -yVel });
		_ball->curY(_gameHeight - ballDiameter);
	}

	if (CollisionDetection::detectCollision(_ball->boundingBox(), _leftPlatform->boundingBox())) {
		float yVel = _ball->velocity().y;
		float xVel = _ball->velocity().x;
		float curY = _ball->curY();

		_ball->setVelocity({ -xVel, yVel });
		//_ball->curY(_gameHeight - ballDiameter);
	}
	else if (CollisionDetection::detectCollision(_ball->boundingBox(), _rightPlatform->boundingBox())) {
		float yVel = _ball->velocity().y;
		float xVel = _ball->velocity().x;
		float curY = _ball->curY();

		_ball->setVelocity({ -xVel, yVel });
		//_ball->curY(_gameHeight - ballDiameter);
	}
}
void Game::gameLoop() {

	auto ticks1 = SDL_GetTicks();
	int frames_now = _frames;

	_leftPlatform->move();
	_rightPlatform->move();
	_ball->move();

	checkCollisions();

	auto time_passed = (SDL_GetTicks() - ticks1);
	SDL_Delay(10);
}

void Game::render() {

	SDL_RenderClear(_renderer);
	
	_leftPlatform->render(_renderer);
	_rightPlatform->render(_renderer);
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
	_frames++;
}

void Game::cleanUp() {
	cout << "End. Cleaning up..." << endl;

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_mainWindow);
	//SDL_FreeSurface(_gameSurface);

	IMG_Quit();
	SDL_Quit();

}




#include <iomanip>
#include <sstream>
#include <iostream>
#include <SDL_image.h>

#include "Game.h"
#include "CollisionDetection.h"
#include "TextureManager.h"

using std::cout;
using std::endl;

void print_FPS(uint32_t time_since_start, int frames);

SDL_Renderer* Game::_renderer = nullptr;

Game::Game() {
	cout << "Game object initialized." << endl;

	_state = GameState::kMainMenu;
	_scoresVector = { 0, 0 };

	_running = true;
	_gameStarted = false;
	_frames = 0;
}

int Game::execute() {
	cout << "Launching game." << endl;

	init("Pong by Jhan", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		600, 400, false);

	SDL_Event e;
	cout << "Starting game..." << endl;
	while (_running) {

		int t1 = SDL_GetTicks();

		while (SDL_PollEvent(&e)) {
			onEvents(&e);
		}
		gameLoop();
		render();

		// Delay to keep FPS consistent
		int t2 = SDL_GetTicks() - t1;
		int ticks_per_frame = 1000 / 60;
		if (t2 < ticks_per_frame) SDL_Delay(ticks_per_frame - t2);
		print_FPS(_timeAtLaunch, _frames);	
	}

	cleanUp();
	return 0;

	
}

bool Game::init(std::string windowName, int xpos, int ypos, int width, int height, bool fullscreen) {
	cout << "Initializing game." << "\n" << std::flush;

	int flag = 0;
	_game_width = width;
	_game_height = height;

	if (fullscreen) flag  = SDL_WINDOW_FULLSCREEN;

	// Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "SDL couldn't initialize! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	// Create Window
	if ((_mainWindow = SDL_CreateWindow(windowName.c_str(), xpos, ypos,
		width, height, flag)) == NULL) {
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

	// Initialize SDL TTF (text render)
	if (TTF_Init() == -1) {
		std::cout << "Failed to initialise SDL_ttf. SDL_ttf error: " << TTF_GetError() << "\n";
	}

	// Load media
	if (loadMedia() == false) {
		return false;
	}

	_timeAtLaunch = SDL_GetTicks();
	
	return true;
}

//	Load textures from image and text
bool Game::loadMedia() {

	_assets.addTexture("ball", "Resources/ball.png");
	_assets.addTexture("leftPlatform", "Resources/plank.bmp");
	_assets.addTexture("rightPlatform", "Resources/plank2.bmp");
	
	_ball = std::make_unique<Ball>(_assets.getTexture("ball"), _game_width / 2, _game_height / 2);
	_leftPlatform = std::make_unique<Platform>(_assets.getTexture("leftPlatform"), 7, 150);
	_rightPlatform = std::make_unique<Platform>(_assets.getTexture("rightPlatform"), _game_width-20, 150);

	_assets.addFont("arl28", "Resources/ARLRDBD.TTF", 28);
	_assets.addFont("arl14", "Resources/ARLRDBD.TTF", 14);

	SDL_Color white = { 255,255,255 };
	
	_scoreLabel = std::make_unique<Label>("Scoreboard", "Resources/ARLRDBD.TTF" , 200, 200, 28, white);
	_countdownTimer = std::make_unique<Label>("Countdown", "Resources/ARLRDBD.TTF",  200, 200, 28, white);
	_controlsLabel = std::make_unique<Label>("Controls", "Resources/ARLRDBD.TTF", 200, 200, 14, white);


	return true;
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
		case SDLK_SPACE:
			if (_state == GameState::kScoreScreen || _state == GameState::kMainMenu) _state = GameState::kPreStart;
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

// Helper function to assist with the correct bounce physics of the ball when in contact with the platforms
void bounceBall(int x, int y, Platform* platform, Ball* ball) {
	int platformLeft;
	int platformRight;
	int platformTop;
	int platformBottom;

	int rectCenterX;
	int rectCenterY;

	// Move ball back one step
	int newX = ball->curX() - ball->velocity().x;
	int newY = ball->curY() - ball->velocity().y;
	
	ball->curX(newX);
	ball->curY(newY);

	// Figure out where from the centre point of rectangle the collision occured
	// Reflect ball away at this angle but keep its y velocity the same (only change y velocity if top/bottom of platform was hit)
	//	*       * O
	//	*        *
	//	* centre *
	//	*        *
	//	*        *
	platformLeft = platform->boundingBox().x;
	platformRight = platform->boundingBox().x + platform->boundingBox().w;
	platformTop = platform->boundingBox().y;
	platformBottom = platform->boundingBox().y + platform->boundingBox().h;

	rectCenterX = (platformLeft + platformRight) / 2;
	rectCenterY = (platformBottom + platformTop) / 2;

	int diffX = x - rectCenterX;
	int diffY = y - rectCenterY;
	int y_magnitude = abs(diffY / diffX);

	int y_dir_ball = ball->velocity().y < 0 ? -1 : 1;

	int direction_multiplierY = 1;

	if (y - ball->velocity().y >= platformBottom || y - ball->velocity().y <= platformTop) direction_multiplierY = -1;	// check if bottom or top of platform was hit

	// Calculate new y velocity
	int yVel = y_magnitude * y_dir_ball * direction_multiplierY;

	// Calculate new x velocity
	int xVel = ball->velocity().x * -1;

	ball->setVelocity({ xVel, yVel });
}

void Game::checkAndReactToPlatformCollisions() {
	// Left platform on boundary
	if (_leftPlatform->curY() < 0) {
		_leftPlatform->curY(0);
		_leftPlatform->stop();
	}
	if ((_leftPlatform->curY() + _leftPlatform->boundingBox().h) > (_game_height)) {
		_leftPlatform->curY(_game_height - _leftPlatform->boundingBox().h);
		_leftPlatform->stop();
	}

	// Right platform on boundary
	if (_rightPlatform->curY() < 0) {
		_rightPlatform->curY(0);
		_rightPlatform->stop();
	}
	if ((_rightPlatform->curY() + _rightPlatform->boundingBox().h) > (_game_height)) {
		_rightPlatform->curY(_game_height - _rightPlatform->boundingBox().h);
		_rightPlatform->stop();
	}

}

void Game::checkAndReactToBallCollisions(int& winner) {

	winner = -1;

	// Ball on boundary
	int ballDiameter = 2 * _ball->boundingBox().r;

	//LEFT
	if (_ball->curX() < 0) {						// PLAYER 2 WINS
		/*_scoresVector[1]++;
		_ball->setVelocity({ 0, 0 });
		_state = GameState::kScoreScreen;
		updateScoreText();*/
		winner = 1;
	}
	//RIGHT
	else if (_ball->curX() > _game_width - ballDiameter) { // PLAYER 1 WINS
		/*_scoresVector[0]++;
		_ball->setVelocity({ 0, 0 });
		_state = GameState::kScoreScreen;
		updateScoreText();*/
		winner = 0;
	}
	//TOP
	else if (_ball->curY() < 0) {
		int yVel = _ball->velocity().y;
		int xVel = _ball->velocity().x;

		_ball->setVelocity({ xVel, -yVel });
		_ball->curY(0);
	}
	//BOTTOM
	else if (_ball->curY() > _game_height - ballDiameter) {

		int yVel = _ball->velocity().y;
		int xVel = _ball->velocity().x;

		_ball->setVelocity({ xVel, -yVel });
		_ball->curY(_game_height - ballDiameter);
	}

	// Ball collision on platforms
	int x = -1;
	int y = -1;
	CollisionDetection::detectCollision(_ball->boundingBox(), _leftPlatform->boundingBox(), x, y);
	if (x != -1 && y != -1) {
		bounceBall(x, y, _leftPlatform.get(), _ball.get());
	}

	CollisionDetection::detectCollision(_ball->boundingBox(), _rightPlatform->boundingBox(), x, y);
	if (x != -1 && y != -1) {
		bounceBall(x, y, _rightPlatform.get(), _ball.get());
	}
}

void Game::gameLoop() {

	int winner = -1;

	switch (_state)
	{
		case GameState::kMainMenu:{
			_scoresVector = { 0, 0 };
			_ball->curX(_game_width / 2);
			_ball->curY(_game_height / 2);
			_leftPlatform->move();
			_rightPlatform->move();
			checkAndReactToPlatformCollisions();

		}break;
		case GameState::kPreStart:{
			_gameStarted = false;
			_ball->setVelocity({ 0,0 });
			_ball->curX(_game_width / 2);
			_ball->curY(_game_height / 2);
			_threadSafeTimer.start(std::chrono::milliseconds(1500));
			_state = GameState::kStart;

		}break;
		case GameState::kStart:{
			if (_threadSafeTimer.isCompleted() == true && _gameStarted == false) {
				_ball->setRandomVelocity();
				_gameStarted = true;
			}

			_leftPlatform->move();
			_rightPlatform->move();
			checkAndReactToPlatformCollisions();
			_ball->move();
			checkAndReactToBallCollisions(winner);

			if (winner != -1) {
				_scoresVector[winner]++;
				_state = GameState::kScoreScreen;
			}
		}break;
		case GameState::kScoreScreen:{
			int a = 1;

		}break;
	}
}


void print_FPS(uint32_t time_since_start, int frames) {
	int t = SDL_GetTicks();
	float fps = (static_cast<float>(frames)*1000) / (t - time_since_start);
	std::cout << "Avg FPS: " << std::setprecision(2) << fps << "\n";
}

std::string getScoreString(std::vector<int> scores) {
	std::ostringstream oss;
	oss << "Score: " << std::setw(5) << std::right << scores[0] << " - " << scores[1];
	return oss.str();
}

void Game::render() {
	SDL_Color white = { 255,255,255 };
	int w = 0;
	int h = 0;

	SDL_RenderClear(_renderer);

	if (_state == GameState::kMainMenu) {
		

		_controlsLabel->changeText("W/S");
		_controlsLabel->show(20, 20);
		 
		_controlsLabel->changeText("UP/DOWN");
		_controlsLabel->show(_game_width - 20 - _controlsLabel->getRect().w, 20);

		_countdownTimer->changeText("Press SPACE to Start");
		_countdownTimer->show(_game_width / 2 - (_countdownTimer->getRect().w / 2), 350);

		SDL_Color white = { 255,255,255 };

		_scoreLabel->changeText(getScoreString(_scoresVector));
		_scoreLabel->show();

	}
	else if (_state == GameState::kScoreScreen) {
		_countdownTimer->changeText("Press SPACE to re-match");
		_countdownTimer->show();
	}

	_leftPlatform->render(_renderer);
	_rightPlatform->render(_renderer);
	_ball->render(_renderer);

	// Render scores
	_scoreLabel->changeText(getScoreString(_scoresVector));
	_scoreLabel->show(_game_width / 2 - (_scoreLabel->getRect().w / 2), 20);

	SDL_SetRenderDrawColor(_renderer, 0x30, 0x30, 0x30, 0xFF);
	SDL_RenderPresent(_renderer);
	_frames++;
	
}

void Game::cleanUp() {
	cout << "End. Cleaning up..." << endl;

	/*for (auto f : _fonts) {
		TTF_CloseFont(f);
		f = NULL;
	}*/

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_mainWindow);
	//SDL_FreeSurface(_gameSurface);

	_renderer = NULL;
	_mainWindow = NULL;

	IMG_Quit();
	SDL_Quit();
	TTF_Quit();

}




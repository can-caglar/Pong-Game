#include <iomanip>
#include <sstream>
#include <iostream>
#include <SDL_image.h>

#include "Game.h"
#include "CollisionDetection.h"
#include "Shapes.h"
#include "Renderer.h"

using std::cout;
using std::endl;

Game::Game() {
	cout << "Game object initialized." << endl;

	_state = GameState::kMainMenu;
	_scoresVector = { 0, 0 };

	/*_ball = std::make_unique<Ball>(GAME_WIDTH / 2, GAME_HEIGHT / 2, 8);
	_leftPlatform = std::make_unique<Platform>(7, 150, 13, 73);
	_rightPlatform = std::make_unique<Platform>(580, 150, 13, 73);*/

	myRenderer = std::make_unique<Renderer>();

	_running = true;
	_gameStarted = false;
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

	//// Init SDL
	//if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
	//	std::cout << "SDL couldn't initialize! SDL_Error: " << SDL_GetError() << "\n";
	//	return false;
	//}

	// Create Window
	//if ((_mainWindow = SDL_CreateWindow("Pong by Can", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	//	myRenderer->GAME_WIDTH, myRenderer->GAME_HEIGHT, SDL_WINDOW_SHOWN)) == NULL) {
	//	return false;
	//}

	//// Create renderer for window
	//_renderer = SDL_CreateRenderer(_mainWindow, -1, SDL_RENDERER_ACCELERATED);
	//if (_renderer == NULL) {
	//	std::cout << "Renderer could not be created.  SDL_Error: " << SDL_GetError() << "\n";
	//	return false;
	//}

	////Initialize PNG loading
	//int imgFlags = IMG_INIT_PNG;
	//if (!(IMG_Init(imgFlags) & imgFlags))
	//{
	//	std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
	//	return false;
	//}

	//// Initialize SDL TTF (text render)
	//if (TTF_Init() == -1) {
	//	std::cout << "Failed to initialise SDL_ttf. SDL_ttf error: " << TTF_GetError() << "\n";
	//}

	//// Load media
	//if (loadMedia() == false) {
	//	return false;
	//}

	_timeAtLaunch = SDL_GetTicks();
	
	return true;
}

////	Load textures from image and text
//bool Game::loadMedia() {
//	
//	auto ball =loadTexture("Resources/ball.png");
//	auto leftP = loadTexture("Resources/plank.bmp");
//	auto rightP = loadTexture("Resources/plank2.bmp");
//
//	//myRenderer = std::make_unique<Renderer>(ball, leftP, rightP); // Create renderer
//	// Has to be in this order
//	myRenderer->addTexture(ball);
//	myRenderer->addTexture(leftP);
//	myRenderer->addTexture(rightP);
//
//	_fonts.push_back(TTF_OpenFont("Resources/ARLRDBD.TTF", 28));
//	if (_fonts[0] == NULL) {
//		std::cout << "Failed to load 28 ARIAL ROUNDED font. SDL_ttf error: " << TTF_GetError() << "\n";
//		return false;
//	}
//
//	_fonts.push_back(TTF_OpenFont("Resources/ARLRDBD.TTF", 14));
//	if (_fonts[1] == NULL) {
//		std::cout << "Failed to load 14 ARIAL ROUNDED font. SDL_ttf error: " << TTF_GetError() << "\n";
//		return false;
//	}
//
//	return true;
//}

//SDL_Texture* Game::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font) {
//
//	SDL_Texture* newTexture = NULL;
//
//	SDL_Surface* textSurface = TTF_RenderText_Blended(font, textureText.c_str(), textColor);
//	if (textSurface == NULL) {
//		std::cout << "Unable to render text surface. SDL_ttf error: " << TTF_GetError() << "\n";
//	}
//	else {
//		newTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);
//		if (newTexture == NULL) {
//			std::cout << "Unable to create texture from rendered text. SDL_ttf error: " << TTF_GetError() << "\n";
//		}
//		else {
//		}
//		SDL_FreeSurface(textSurface);
//	}
//	return newTexture;
//}

//SDL_Texture* Game::loadTexture(std::string path) {
//	SDL_Texture* newTexture = NULL;
//
//	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
//	if (loadedSurface == NULL) {
//		std::cout << "Unable to load image " << path << ". SDL_image error: " << IMG_GetError() << "\n";
//	}
//	else {
//		newTexture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
//		if (newTexture == NULL) {
//			std::cout << "Unable to create texture from " << path << ". SDL Error: " << SDL_GetError() << "\n";
//		}
//		SDL_FreeSurface(loadedSurface);
//	}
//	return newTexture;
//}

void Game::onEvents(SDL_Event* event) {
	if (event->type == SDL_QUIT) {
		_running = false;
	}
	else if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym)
		{
		case SDLK_UP:
			myRenderer->_rightPlatform->moveUp();
			break;
		case SDLK_DOWN:
			myRenderer->_rightPlatform->moveDown();
			break;
		case SDLK_w:
			myRenderer->_leftPlatform->moveUp();
			break;
		case SDLK_s:
			myRenderer->_leftPlatform->moveDown();
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
			myRenderer->_rightPlatform->stop();
			break;
		case SDLK_DOWN:
			myRenderer->_rightPlatform->stop();
			break;
		case SDLK_w:
			myRenderer->_leftPlatform->stop();
			break;
		case SDLK_s:
			myRenderer->_leftPlatform->stop();
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
	if (myRenderer->_leftPlatform->curY() < 0) {
		myRenderer->_leftPlatform->curY(0);
		myRenderer->_leftPlatform->stop();
	}
	if ((myRenderer->_leftPlatform->curY() + myRenderer->_leftPlatform->boundingBox().h) > (myRenderer->GAME_HEIGHT)) {
		myRenderer->_leftPlatform->curY(myRenderer->GAME_HEIGHT - myRenderer->_leftPlatform->boundingBox().h);
		myRenderer->_leftPlatform->stop();
	}

	// Right platform on boundary
	if (myRenderer->_rightPlatform->curY() < 0) {
		myRenderer->_rightPlatform->curY(0);
		myRenderer->_rightPlatform->stop();
	}
	if ((myRenderer->_rightPlatform->curY() + myRenderer->_rightPlatform->boundingBox().h) > (myRenderer->GAME_HEIGHT)) {
		myRenderer->_rightPlatform->curY(myRenderer->GAME_HEIGHT - myRenderer->_rightPlatform->boundingBox().h);
		myRenderer->_rightPlatform->stop();
	}

}

void Game::checkAndReactToBallCollisions(int& winner) {

	winner = -1;

	// Ball on boundary
	int ballDiameter = 2 * myRenderer->_ball->boundingBox().r;

	//LEFT
	if (myRenderer->_ball->curX() < 0) {						// PLAYER 2 WINS
		/*_scoresVector[1]++;
		_ball->setVelocity({ 0, 0 });
		_state = GameState::kScoreScreen;
		updateScoreText();*/
		winner = 1;
	}
	//RIGHT
	else if (myRenderer->_ball->curX() > myRenderer->GAME_WIDTH - ballDiameter) { // PLAYER 1 WINS
		/*_scoresVector[0]++;
		_ball->setVelocity({ 0, 0 });
		_state = GameState::kScoreScreen;
		updateScoreText();*/
		winner = 0;
	}
	//TOP
	else if (myRenderer->_ball->curY() < 0) {
		int yVel = myRenderer->_ball->velocity().y;
		int xVel = myRenderer->_ball->velocity().x;

		myRenderer->_ball->setVelocity({ xVel, -yVel });
		myRenderer->_ball->curY(0);
	}
	//BOTTOM
	else if (myRenderer->_ball->curY() > myRenderer->GAME_HEIGHT - ballDiameter) {

		int yVel = myRenderer->_ball->velocity().y;
		int xVel = myRenderer->_ball->velocity().x;

		myRenderer->_ball->setVelocity({ xVel, -yVel });
		myRenderer->_ball->curY(myRenderer->GAME_HEIGHT - ballDiameter);
	}

	// Ball collision on platforms
	int x = -1;
	int y = -1;
	CollisionDetection::detectCollision(myRenderer->_ball->boundingBox(), myRenderer->_leftPlatform->boundingBox(), x, y);
	if (x != -1 && y != -1) {
		bounceBall(x, y, myRenderer->_leftPlatform.get(), myRenderer->_ball.get());
	}

	CollisionDetection::detectCollision(myRenderer->_ball->boundingBox(), myRenderer->_rightPlatform->boundingBox(), x, y);
	if (x != -1 && y != -1) {
		bounceBall(x, y, myRenderer->_rightPlatform.get(), myRenderer->_ball.get());
	}
}

void Game::gameLoop() {

	int winner = -1;

	switch (_state)
	{
		case GameState::kMainMenu:{
			_scoresVector = { 0, 0 };
			myRenderer->_leftPlatform->move();
			myRenderer->_rightPlatform->move();
			checkAndReactToPlatformCollisions();

		}break;
		case GameState::kPreStart:{
			_gameStarted = false;
			myRenderer->_ball->setVelocity({ 0,0 });
			myRenderer->_ball->curX(myRenderer->GAME_WIDTH / 2);
			myRenderer->_ball->curY(myRenderer->GAME_HEIGHT / 2);
			_threadSafeTimer.start(std::chrono::milliseconds(1500));
			_state = GameState::kStart;

		}break;
		case GameState::kStart:{
			if (_threadSafeTimer.isCompleted() == true && _gameStarted == false) {
				myRenderer->_ball->setRandomVelocity();
				_gameStarted = true;
			}

			myRenderer->_leftPlatform->move();
			myRenderer->_rightPlatform->move();
			checkAndReactToPlatformCollisions();
			myRenderer->_ball->move();
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
//
//void Game::updateScoreTextTure() {
//	
//	std::ostringstream oss;
//	oss << "Score: " << std::setw(5) << std::right  << _scoresVector[0] << " - " << _scoresVector[1];
//	SDL_Color white = { 255,255,255 };
//
//	if (_scoresTexture != NULL) SDL_DestroyTexture(_scoresTexture);
//	_scoresTexture = loadFromRenderedText(oss.str().c_str(), white, _fonts[0]);
//
//	if (_scoresTexture == NULL) {
//		std::cout << "Failed to change _scoresTexture texture \n";
//	}
//}

//
//void Game::renderText(SDL_Texture* tt, int xpos, int ypos) {
//	
//	if (tt == NULL) return;
//
//	int w=140;
//	int h=40;
//
//	SDL_QueryTexture(tt, NULL, NULL, &w, &h);
//
//	SDL_Rect newPos = { xpos, ypos , w, h };
//	SDL_RenderCopy(_renderer, tt, NULL, &newPos);
//}

void print_FPS(uint32_t time_since_start, int frames) {
	int t = SDL_GetTicks();
	float fps = (static_cast<float>(frames)*1000) / (t - time_since_start);
	std::cout << "Avg FPS: " << std::setprecision(2) << fps << "\n";
}

void Game::render() {
	SDL_Color white = { 255,255,255 };
	int t1 = SDL_GetTicks();
	int w = 0;
	int h = 0;

	SDL_RenderClear(myRenderer->_renderer);

	if (_state == GameState::kMainMenu) {
		
		_controlsTexture = myRenderer->loadFromRenderedText("W/S", white, myRenderer->_fonts[1]);
		myRenderer->renderText(_controlsTexture, 20, 20);

		_controlsTexture = myRenderer->loadFromRenderedText("UP/DOWN", white, myRenderer->_fonts[1]);
		w = 0;
		SDL_QueryTexture(_controlsTexture, NULL, NULL, &w, NULL);
		myRenderer->renderText(_controlsTexture, 580-w, 20);

		_countdownTimer = myRenderer->loadFromRenderedText("Press SPACE to Start", white, myRenderer->_fonts[0]);
		SDL_QueryTexture(_countdownTimer, NULL, NULL, &w, NULL);
		myRenderer->renderText(_countdownTimer, myRenderer->GAME_WIDTH / 2 - (w / 2), 350);

		//updateScoreTextTure();
	}
	else if (_state == GameState::kScoreScreen) {
		//updateScoreTextTure();
		_countdownTimer = myRenderer->loadFromRenderedText("Press SPACE to re-match", white, myRenderer->_fonts[1]);
		SDL_QueryTexture(_countdownTimer, NULL, NULL, &w, NULL);
		myRenderer->renderText(_countdownTimer, myRenderer->GAME_WIDTH / 2 - (w / 2), 350);
	}

	//_leftPlatform->render(_renderer);
	//_rightPlatform->render(_renderer);
	//_ball->render(_renderer);
	myRenderer->render();

	// Render scores
	SDL_QueryTexture(_scoresTexture, NULL, NULL, &w, NULL);
	myRenderer->renderText(_scoresTexture, myRenderer->GAME_WIDTH/2 - (w/2), 20);

	SDL_SetRenderDrawColor(myRenderer->_renderer, 0x30, 0x30, 0x30, 0xFF);
	SDL_RenderPresent(myRenderer->_renderer);
	_frames++;

	// Delay to keep FPS consistent
	int t2 = SDL_GetTicks() - t1;
	int ticks_per_frame = 1000 / 60;
	if (t2 < ticks_per_frame) SDL_Delay(ticks_per_frame - t2);
	print_FPS(_timeAtLaunch, _frames);
}

void Game::cleanUp() {
	cout << "End. Cleaning up..." << endl;
}




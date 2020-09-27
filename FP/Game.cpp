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

	//myRenderer = std::make_unique<Renderer>();
	mySceneManager = std::make_unique<SceneManager>();

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

	_timeAtLaunch = SDL_GetTicks();
	
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
			mySceneManager->myRenderer->_rightPlatform->moveUp();
			break;
		case SDLK_DOWN:
			mySceneManager->myRenderer->_rightPlatform->moveDown();
			break;
		case SDLK_w:
			mySceneManager->myRenderer->_leftPlatform->moveUp();
			break;
		case SDLK_s:
			mySceneManager->myRenderer->_leftPlatform->moveDown();
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
			mySceneManager->myRenderer->_rightPlatform->stop();
			break;
		case SDLK_DOWN:
			mySceneManager->myRenderer->_rightPlatform->stop();
			break;
		case SDLK_w:
			mySceneManager->myRenderer->_leftPlatform->stop();
			break;
		case SDLK_s:
			mySceneManager->myRenderer->_leftPlatform->stop();
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
	if (mySceneManager->myRenderer->_leftPlatform->curY() < 0) {
		mySceneManager->myRenderer->_leftPlatform->curY(0);
		mySceneManager->myRenderer->_leftPlatform->stop();
	}
	if ((mySceneManager->myRenderer->_leftPlatform->curY() + mySceneManager->myRenderer->_leftPlatform->boundingBox().h) > (mySceneManager->myRenderer->GAME_HEIGHT)) {
		mySceneManager->myRenderer->_leftPlatform->curY(mySceneManager->myRenderer->GAME_HEIGHT - mySceneManager->myRenderer->_leftPlatform->boundingBox().h);
		mySceneManager->myRenderer->_leftPlatform->stop();
	}

	// Right platform on boundary
	if (mySceneManager->myRenderer->_rightPlatform->curY() < 0) {
		mySceneManager->myRenderer->_rightPlatform->curY(0);
		mySceneManager->myRenderer->_rightPlatform->stop();
	}
	if ((mySceneManager->myRenderer->_rightPlatform->curY() + mySceneManager->myRenderer->_rightPlatform->boundingBox().h) > (mySceneManager->myRenderer->GAME_HEIGHT)) {
		mySceneManager->myRenderer->_rightPlatform->curY(mySceneManager->myRenderer->GAME_HEIGHT - mySceneManager->myRenderer->_rightPlatform->boundingBox().h);
		mySceneManager->myRenderer->_rightPlatform->stop();
	}

}

void Game::checkAndReactToBallCollisions(int& winner) {

	winner = -1;

	// Ball on boundary
	int ballDiameter = 2 * mySceneManager->myRenderer->_ball->boundingBox().r;

	//LEFT
	if (mySceneManager->myRenderer->_ball->curX() < 0) {						// PLAYER 2 WINS
		/*_scoresVector[1]++;
		_ball->setVelocity({ 0, 0 });
		_state = GameState::kScoreScreen;
		updateScoreText();*/
		winner = 1;
	}
	//RIGHT
	else if (mySceneManager->myRenderer->_ball->curX() > mySceneManager->myRenderer->GAME_WIDTH - ballDiameter) { // PLAYER 1 WINS
		/*_scoresVector[0]++;
		_ball->setVelocity({ 0, 0 });
		_state = GameState::kScoreScreen;
		updateScoreText();*/
		winner = 0;
	}
	//TOP
	else if (mySceneManager->myRenderer->_ball->curY() < 0) {
		int yVel = mySceneManager->myRenderer->_ball->velocity().y;
		int xVel = mySceneManager->myRenderer->_ball->velocity().x;

		mySceneManager->myRenderer->_ball->setVelocity({ xVel, -yVel });
		mySceneManager->myRenderer->_ball->curY(0);
	}
	//BOTTOM
	else if (mySceneManager->myRenderer->_ball->curY() > mySceneManager->myRenderer->GAME_HEIGHT - ballDiameter) {

		int yVel = mySceneManager->myRenderer->_ball->velocity().y;
		int xVel = mySceneManager->myRenderer->_ball->velocity().x;

		mySceneManager->myRenderer->_ball->setVelocity({ xVel, -yVel });
		mySceneManager->myRenderer->_ball->curY(mySceneManager->myRenderer->GAME_HEIGHT - ballDiameter);
	}

	// Ball collision on platforms
	int x = -1;
	int y = -1;
	CollisionDetection::detectCollision(mySceneManager->myRenderer->_ball->boundingBox(), mySceneManager->myRenderer->_leftPlatform->boundingBox(), x, y);
	if (x != -1 && y != -1) {
		bounceBall(x, y, mySceneManager->myRenderer->_leftPlatform.get(), mySceneManager->myRenderer->_ball.get());
	}

	CollisionDetection::detectCollision(mySceneManager->myRenderer->_ball->boundingBox(), mySceneManager->myRenderer->_rightPlatform->boundingBox(), x, y);
	if (x != -1 && y != -1) {
		bounceBall(x, y, mySceneManager->myRenderer->_rightPlatform.get(), mySceneManager->myRenderer->_ball.get());
	}
}

void Game::gameLoop() {

	int winner = -1;

	switch (_state)
	{
		case GameState::kMainMenu:{
			_scoresVector = { 0, 0 };
			mySceneManager->myRenderer->_leftPlatform->move();
			mySceneManager->myRenderer->_rightPlatform->move();
			checkAndReactToPlatformCollisions();

		}break;
		case GameState::kPreStart:{
			_gameStarted = false;
			mySceneManager->myRenderer->_ball->setVelocity({ 0,0 });
			mySceneManager->myRenderer->_ball->curX(mySceneManager->myRenderer->GAME_WIDTH / 2);
			mySceneManager->myRenderer->_ball->curY(mySceneManager->myRenderer->GAME_HEIGHT / 2);
			mySceneManager->_threadSafeTimer.start(std::chrono::milliseconds(1500));
			_state = GameState::kStart;

		}break;
		case GameState::kStart:{
			if (mySceneManager->_threadSafeTimer.isCompleted() == true && _gameStarted == false) {
				mySceneManager->myRenderer->_ball->setRandomVelocity();
				_gameStarted = true;
			}

			mySceneManager->myRenderer->_leftPlatform->move();
			mySceneManager->myRenderer->_rightPlatform->move();
			checkAndReactToPlatformCollisions();
			mySceneManager->myRenderer->_ball->move();
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

void Game::render() {
	SDL_Color white = { 255,255,255 };
	int t1 = SDL_GetTicks();
	int w = 0;
	int h = 0;

	SDL_RenderClear(mySceneManager->myRenderer->_renderer);

	if (_state == GameState::kMainMenu) {
		
		mySceneManager->_controlsTexture = mySceneManager->myRenderer->myTextureManager->loadTexture("W/S", white, mySceneManager->myRenderer->myTextureManager->_fonts[1], mySceneManager->myRenderer->_renderer);
		mySceneManager->myRenderer->renderThis(mySceneManager->_controlsTexture, 20, 20);

		mySceneManager->_controlsTexture = mySceneManager->myRenderer->myTextureManager->loadTexture("UP/DOWN", white, mySceneManager->myRenderer->myTextureManager->_fonts[1], mySceneManager->myRenderer->_renderer);
		w = 0;
		SDL_QueryTexture(mySceneManager->_controlsTexture, NULL, NULL, &w, NULL);
		mySceneManager->myRenderer->renderThis(mySceneManager->_controlsTexture, 580-w, 20);

		mySceneManager->_countdownTimer = mySceneManager->myRenderer->myTextureManager->loadTexture("Press SPACE to Start", white, mySceneManager->myRenderer->myTextureManager->_fonts[0], mySceneManager->myRenderer->_renderer);
		SDL_QueryTexture(mySceneManager->_countdownTimer, NULL, NULL, &w, NULL);
		mySceneManager->myRenderer->renderThis(mySceneManager->_countdownTimer, mySceneManager->myRenderer->GAME_WIDTH / 2 - (w / 2), 350);

		//updateScoreTextTure();
	}
	else if (_state == GameState::kScoreScreen) {
		//updateScoreTextTure();
		mySceneManager->_countdownTimer = mySceneManager->myRenderer->myTextureManager->loadTexture("Press SPACE to re-match", white, mySceneManager->myRenderer->myTextureManager->_fonts[1], mySceneManager->myRenderer->_renderer);
		SDL_QueryTexture(mySceneManager->_countdownTimer, NULL, NULL, &w, NULL);
		mySceneManager->myRenderer->renderThis(mySceneManager->_countdownTimer, mySceneManager->myRenderer->GAME_WIDTH / 2 - (w / 2), 350);
	}

	//_leftPlatform->render(_renderer);
	//_rightPlatform->render(_renderer);
	//_ball->render(_renderer);
	mySceneManager->myRenderer->render();

	// Render scores
	SDL_QueryTexture(mySceneManager->_scoresTexture, NULL, NULL, &w, NULL);
	mySceneManager->myRenderer->renderThis(mySceneManager->_scoresTexture, mySceneManager->myRenderer->GAME_WIDTH/2 - (w/2), 20);

	SDL_SetRenderDrawColor(mySceneManager->myRenderer->_renderer, 0x30, 0x30, 0x30, 0xFF);
	SDL_RenderPresent(mySceneManager->myRenderer->_renderer);
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




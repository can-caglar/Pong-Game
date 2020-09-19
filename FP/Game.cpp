#include "Game.h"
#include "CollisionDetection.h"

using std::cout;
using std::endl;


Game::Game() {
	cout << "Game object initialized." << endl;

	_state = GameState::kMainMenu;
	_scoresVector = { 0, 0 };
	_ball = std::make_unique<Ball>(GAME_WIDTH / 2, GAME_HEIGHT / 2, 7.5f);

	_leftPlatform = std::make_unique<Platform>(7, 150, 13, 73);
	_rightPlatform = std::make_unique<Platform>(580, 150, 13, 73);

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

	// Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "SDL couldn't initialize! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	// Create Window
	if ((_mainWindow = SDL_CreateWindow("Pong by Can", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_SHOWN)) == NULL) {
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
	if (!TTF_Init() == -1) {
		std::cout << "Failed to initialise SDL_ttf. SDL_ttf error: " << TTF_GetError() << "\n";
	}

	// Load media
	if (loadMedia() == false) {
		return false;
	}
	
	return true;
}

//	Load textures from image and text
bool Game::loadMedia() {
	
	_ball->setTexture(loadTexture("Resources/ball.png"));
	_leftPlatform->setTexture(loadTexture("Resources/plank.bmp"));
	_rightPlatform->setTexture(loadTexture("Resources/plank2.bmp"));

	_font = TTF_OpenFont("Resources/ARLRDBD.TTF", 28);
	if (_font == NULL) {
		std::cout << "Failed to load ARIAL ROUNDED font. SDL_ttf error: " << TTF_GetError() << "\n";
	}
	else {
		SDL_Color textColor = { 255,255, 255 };
		_scoresTexture = loadFromRenderedText("SCORES", textColor);
		if (!_scoresTexture) {
			std::cout << "Failed to render text texture! \n";
			return false;
		}
		_countdownTimer = loadFromRenderedText("COUNTDOWN", textColor);
		if (!_countdownTimer) {
			std::cout << "Failed to render countdown texture! \n";
			return false;
		}
	}

	return true;
}

SDL_Texture* Game::loadFromRenderedText(std::string textureText, SDL_Color textColor) {

	SDL_Texture* newTexture = NULL;

	SDL_Surface* textSurface = TTF_RenderText_Blended(_font, textureText.c_str(), textColor);
	if (textSurface == NULL) {
		std::cout << "Unable to render text surface. SDL_ttf error: " << TTF_GetError() << "\n";
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);
		if (newTexture == NULL) {
			std::cout << "Unable to create texture from rendered text. SDL_ttf error: " << TTF_GetError() << "\n";
		}
		else {
			//TODO / NOTE: textSurface->w gives the width of the rendered surface. use this to catch the ball/blank sizes.
		}
		SDL_FreeSurface(textSurface);
	}
	return newTexture;
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

void bounceBall(float x, float y, Platform* platform, Ball* ball) {
	float platformLeft;
	float platformRight;
	float platformTop;
	float platformBottom;

	float rectCenterX;
	float rectCenterY;

	float newXPos;
	float newYPos;

	// Move ball back one step

	// Figure out where from the centre of rectangle the collision occured
	// Reflect ball away at this angle (only change y velocity if top/bottom of platform was hit)
	//	*   * O
	//	*   *
	//	* c *
	//	*   *
	//	*   *

	platformLeft = platform->boundingBox().x;
	platformRight = platform->boundingBox().x + platform->boundingBox().w;
	platformTop = platform->boundingBox().y;
	platformBottom = platform->boundingBox().y + platform->boundingBox().h;

	rectCenterX = (platformLeft + platformRight) / 2;
	rectCenterY = (platformBottom + platformTop) / 2;

	float diffX = fabs(x - rectCenterX);
	float diffY = fabs(y - rectCenterY);
	
	// Calculate y velocity
	float yVel = diffY / diffX;
	if (ball->velocity().y < -1) yVel *= -1;	// check which direction the ball was going in the y axis
	if (y - ball->velocity().y >= platformBottom || y - ball->velocity().y <= platformTop)
	{
		yVel *= -1;								// if top or bottom of platform hit, change y direction
	}
	
	// Calculate x velocity
	float xVel = -ball->velocity().x;

	ball->setVelocity({ xVel, yVel });
}

void Game::checkCollisions() {

	// Left platform on boundary
	if (_leftPlatform->curY() < 0) {
		_leftPlatform->curY(0);
		_leftPlatform->stop();
	}
	if ((_leftPlatform->curY() + _leftPlatform->boundingBox().h) > (GAME_HEIGHT)) {
		_leftPlatform->curY(GAME_HEIGHT - _leftPlatform->boundingBox().h);
		_leftPlatform->stop();
	}

	// Right platform on boundary
	if (_rightPlatform->curY() < 0) {
		_rightPlatform->curY(0);
		_rightPlatform->stop();
	}
	if ((_rightPlatform->curY() + _rightPlatform->boundingBox().h) > (GAME_HEIGHT)) {
		_rightPlatform->curY(GAME_HEIGHT - _rightPlatform->boundingBox().h);
		_rightPlatform->stop();
	}

	// Ball on boundary
	float ballDiameter = 2 * _ball->boundingBox().r;
	if (_ball->curX() < 0) {
		// PLAYER 2 WINS
		_scoresVector[1]++;
		_ball->setVelocity({ 0, 0 });
		_state = GameState::kScoreScreen;

		updateScoreText();
	}
	if (_ball->curX() > GAME_WIDTH - ballDiameter) {
		// PLAYER 1 WINS
		_scoresVector[0]++;
		_ball->setVelocity({ 0, 0 });
		_state = GameState::kScoreScreen;
		updateScoreText();
	}
	if (_ball->curY() < 0) {
		float yVel = _ball->velocity().y;
		float xVel = _ball->velocity().x;

		_ball->setVelocity({ xVel, -yVel });
		_ball->curY(0);
	}
	if (_ball->curY() > GAME_HEIGHT - ballDiameter) {

		float yVel = _ball->velocity().y;
		float xVel = _ball->velocity().x;

		_ball->setVelocity({ xVel, -yVel });
		_ball->curY(GAME_HEIGHT - ballDiameter);
	}

	// Ball collision on platforms
	float x = -1;
	float y = -1;
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

	switch (_state)
	{
		case GameState::kMainMenu:{
			_scoresVector = { 0, 0 };
			_state = GameState::kPreStart;

		}break;
		case GameState::kPreStart:{
			_gameStarted = false;
			_ball->setVelocity({ 0,0 });
			_ball->curX(GAME_WIDTH / 2);
			_ball->curY(GAME_HEIGHT / 2);
			_threadSafeTimer.start(std::chrono::milliseconds(3000));
			_state = GameState::kStart;

			updateScoreText();

		}break;
		case GameState::kStart:{
			if (_threadSafeTimer.isCompleted() == true && _gameStarted == false) {
				_ball->setRandomVelocity();
				_gameStarted = true;
			}

			_leftPlatform->move();
			_rightPlatform->move();
			_ball->move();
			checkCollisions();

		}break;
		case GameState::kScoreScreen:{
			int a = 1;

		}break;
	}

	std::cout << "Player 1 = " << _scoresVector[0] << " Player 2 = " << _scoresVector[1] << "\n";
	auto ticks1 = SDL_GetTicks();
	int frames_now = _frames;
}

void Game::updateScoreText() {
	
	std::ostringstream oss;
	oss << "Score: " << std::setw(5) << std::right  << _scoresVector[0] << " - " << _scoresVector[1];
	SDL_Color white = { 255,255,255 };

	if (_scoresTexture != NULL) SDL_DestroyTexture(_scoresTexture);
	_scoresTexture = loadFromRenderedText(oss.str().c_str(), white);

	if (_scoresTexture == NULL) {
		std::cout << "Failed to change _scoresTexture texture \n";
	}
}

void Game::renderText(SDL_Texture* tt, float xpos, float ypos) {
	
	if (tt == NULL) return;

	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0XFF, 0XFF);

	int w=140;
	int h=40;

	SDL_QueryTexture(tt, NULL, NULL, &w, &h);

	SDL_Rect newPos = { xpos, ypos , w, h };
	SDL_RenderCopy(_renderer, tt, NULL, &newPos);
}

void Game::render() {

	int t1 = SDL_GetTicks();

	SDL_RenderClear(_renderer);

	_leftPlatform->render(_renderer);
	_rightPlatform->render(_renderer);
	_ball->render(_renderer);
	renderText(_scoresTexture, 180, 20);

	SDL_SetRenderDrawColor(_renderer, 0x30, 0x30, 0x30, 0xFF);

	SDL_RenderPresent(_renderer);
	_frames++;
	int t2 = SDL_GetTicks() - t1;
	SDL_Delay(10);
	std::cout << "Time taken for render: "  << SDL_GetTicks() - t1<< "ms. Frames done : " << _frames << "\n";
}

void Game::cleanUp() {
	cout << "End. Cleaning up..." << endl;

	TTF_CloseFont(_font);
	_font = NULL;

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_mainWindow);
	//SDL_FreeSurface(_gameSurface);

	_renderer = NULL;
	_mainWindow = NULL;

	IMG_Quit();
	SDL_Quit();
	TTF_Quit();

}




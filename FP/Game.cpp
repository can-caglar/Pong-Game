#include "Game.h"

using std::cout;
using std::endl;

Game::Game() {
	cout << "Game object initialized." << endl;
	_running = true;
}

int Game::execute() {
	cout << "Launching game." << endl;

	init();

	SDL_Event e;
	cout << "Starting game..." << endl;
	while (_running) {
		while (SDL_PollEvent(&e)) {
			events(e);
		}
		gameLoop();
		render();
	}

	cleanUp();
	return 0;
}

bool Game::init() {
	cout << "Initializing data." << endl;
	return true;
}

void Game::events(SDL_Event event) {

}

void Game::gameLoop() {

}

void Game::render() {

}

void Game::cleanUp() {
	cout << "End. Cleaning up..." << endl;
}




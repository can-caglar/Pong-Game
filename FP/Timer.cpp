#include "Timer.h"

void Timer::delay(const std::chrono::milliseconds& ms) {
	std::unique_lock<std::mutex> lck(mtx);
	_completed = false;

	_isRunning = true;

	lck.unlock();
	auto time_started = steady_clock::now();

	std::this_thread::sleep_for(ms);

	lck.lock();
	_isRunning = false;
	_completed = true;
}

bool Timer::isRunning() {
	std::unique_lock<std::mutex> lck(mtx);
	return _isRunning;
}

bool Timer::isCompleted() {
	std::unique_lock<std::mutex> lck(mtx);
	return _completed;
}

bool Timer::start(const std::chrono::milliseconds& ms) {
	if (isRunning()) {
		return false;
	}
	else {
		_ftr = std::async(&Timer::delay, this, ms);
		return true;
	}
}
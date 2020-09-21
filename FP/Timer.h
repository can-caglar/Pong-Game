// Thread safe timer class

#pragma once
#include <chrono>
#include <mutex>
#include <iostream>
#include <future>

using std::chrono::steady_clock;

class Timer
{

private:
	std::mutex mtx;
	std::future<void> _ftr;

	bool _isRunning;
	bool _completed;

	void delay(const std::chrono::milliseconds& ms) {
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

public:
	Timer() : _isRunning(false), _completed(false) {};

	bool isRunning() {
		std::unique_lock<std::mutex> lck(mtx);
		return _isRunning;
	}

	bool isCompleted() {
		std::unique_lock<std::mutex> lck(mtx);
		return _completed;
	}

	// @brief: Starts a timer in a separate thread. Read from isCompleted() to check if timer successfully stopped running.
	// @return: True if timer successfully started. False if time is already running.
	bool start(const std::chrono::milliseconds& ms) {
		if (isRunning()) {
			return false;
		}
		else {
			_ftr = std::async(&Timer::delay, this, ms);
			return true;
		}
	}
};

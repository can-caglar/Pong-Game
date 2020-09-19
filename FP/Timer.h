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

		std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;

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

	void start(const std::chrono::milliseconds& ms) {
		_ftr = std::async(&Timer::delay, this, ms);
	}
};

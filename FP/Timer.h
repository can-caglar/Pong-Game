#pragma once

#include <future>
#include <chrono>
#include <mutex>

using std::chrono::steady_clock;

class Timer
{

public:
	Timer() : _isRunning(false), _completed(false) {};

	bool isRunning();
	bool isCompleted();
	bool start(const std::chrono::milliseconds& ms);

private:
	std::mutex mtx;
	std::future<void> _ftr;

	bool _isRunning;
	bool _completed;
	void delay(const std::chrono::milliseconds& ms);

};

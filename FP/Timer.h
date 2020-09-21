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
	void delay(const std::chrono::milliseconds& ms);

public:
	Timer() : _isRunning(false), _completed(false) {};

	bool isRunning();
	bool isCompleted();
	bool start(const std::chrono::milliseconds& ms);
};

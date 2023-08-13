#pragma once
#include "Observer.h"

class Timer : public Subject
{
	typedef std::shared_ptr<Timer> TimerPtr;

private:
	static TimerPtr instance;
	static std::mutex mutex_;

//protected:
	Timer(size_t frequency_ms) {}
	~Timer() {}
	size_t _frequency_ms;

public:
	//Timer(Timer& other) = delete;
	//void operator=(const Timer &) = delete;
	static TimerPtr GetInstance(size_t frequency_ms);
	void tick();
};

typedef std::shared_ptr<Timer> TimerPtr;

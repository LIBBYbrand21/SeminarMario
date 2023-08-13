#include "Timer.h"
#include "opencv2/opencv.hpp"
#include "Config.h"
using namespace std;

TimerPtr Timer::instance{nullptr};

mutex Timer::mutex_;

TimerPtr Timer::GetInstance(size_t frequency_ms) {
	//lock_guard<mutex> lock(mutex_);
	if (instance == nullptr)
	{
		instance = new Timer(frequency_ms);
	}
	return instance;
}
Timer::Timer(size_t frequency_ms)
	:_frequency_ms(frequency_ms)
{
}

void Timer::tick()
{
	int key = cv::waitKey(_frequency_ms);

	Notify(Event{ EventSenders::SENDER_TIMER, EventTypes::EVENT_TIMER , EventCodes::TIME_TICK });
	if (key != -1)
		Notify(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED , key });
}

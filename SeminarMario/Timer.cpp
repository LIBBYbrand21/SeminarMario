#include "Timer.h"
#include "opencv2/opencv.hpp"
#include "Config.h"
using namespace std;

TimerPtr Timer::instance = nullptr;

TimerPtr Timer::GetInstance(size_t frequency_ms)
{
	if (instance == nullptr)
		instance = make_shared<Timer>(Timer(frequency_ms));
	return instance;
}

Timer::Timer(size_t frequency_ms)
	:_frequency_ms(frequency_ms), timeToFlickering(-1)
{
}

void Timer::tick()
{
	int key = cv::waitKey(_frequency_ms);

	Notify(Event{ EventSenders::SENDER_TIMER, EventTypes::EVENT_TIMER , EventCodes::TIME_TICK });
	if (key != -1)
		Notify(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED , key });
	if (isTimeToFlicering()) {
		timeToFlickering++;
		if (timeToFlickering == 10) {
			Notify(Event{ EventSenders::SENDER_TIMER, EventTypes::EVENT_TIMER , EventCodes::TIME_FLICKERING_FINISH });
			timeToFlickering = -1;
		}
}
}

bool Timer::isTimeToFlicering()
{
	return timeToFlickering!= -1;
}

void Timer::setTimeToFlicering()
{
	timeToFlickering++;
}

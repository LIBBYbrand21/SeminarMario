#include "Animation.h"
#include <opencv2/opencv.hpp>
#define HERO_DUCK 's'
#define HERO_JUMP 'w'
#define HERO_STAND_AFTER_DUCK 'w'
#define HERO_RUN_RIGHT 'd'
#define HERO_IDLE 'a'

cv::Mat background = cv::imread(R"(../Animations/background.png)", cv::IMREAD_UNCHANGED);
bool isToExit = false;


int walkRight(cv::Point& topLeft);
int idle(cv::Point& topLeft);
int duck(cv::Point& topLeft);
int jumpRight(cv::Point& topLeft);


int main()
{
	cv::resize(background, background, cv::Size(), 0.79, 0.75);
	cv::Point topLeft(background.size().width / 2, background.size().height * 2.03 / 3);

	idle(topLeft);

	while (false == isToExit)
	{
		int key = cv::waitKey(100);
		switch (key)
		{
		case 27: isToExit = true;
		case HERO_DUCK: duck(topLeft);
		case HERO_JUMP: key = jumpRight(topLeft);
		case HERO_RUN_RIGHT:key = walkRight(topLeft);
		case HERO_IDLE:key = idle(topLeft);
		}
	}


	return 0;
}
void show(Animation& animation, cv::Point& topLeft, int x = 0, int y = 0) {
	for (int i = 0; i < animation.numFrames(); i++)
	{
		Frame const& frame = animation.getFrame(i);
		cv::Mat canvas = background.clone();
		topLeft.x += x;
		if (i < 5) {
			topLeft.y -= y;
		}
		else {
			topLeft.y += y;
		}
		if (topLeft.x > background.size().width - 10) {
			topLeft.x = 0;
		}
		drawFrame(frame, canvas, topLeft);
		cv::imshow("test", canvas);
		int key = cv::waitKey(100);
	}
}

int walkRight(cv::Point& topLeft) {
	Animation animation(R"(../Animations/Hero/runRight)");
	int key = cv::waitKey(100);
	while (key == -1) {
		show(animation, topLeft, 10);
		int key = cv::waitKey(100);
		if (key > 0) {
			return key;
		}
	}
}

int idle(cv::Point& topLeft) {
	Animation animation(R"(../Animations/Hero/idle)");
	int key = cv::waitKey(100);
	while (key == -1) {
		show(animation, topLeft);
		key = cv::waitKey(100);
		if (key > 0) {
			return key;
		}
	}

}

int duck(cv::Point& topLeft)
{
	Animation animation(R"(../Animations/Hero/duckDown)");
	show(animation,topLeft);
	animation = Animation(R"(../Animations/Hero/duckStay)");
	int key = cv::waitKey(100);
	while (key != HERO_STAND_AFTER_DUCK) {
		Frame const& frame = animation.getFrame(0);
		cv::Mat canvas = background.clone();
		drawFrame(frame, canvas, topLeft);
		cv::imshow("test", canvas);
		int key = cv::waitKey(100);
		if (key == HERO_STAND_AFTER_DUCK) {
			break;
		}
		if (key == 27) {
			isToExit = true;
			return key;
		}
	}
	animation = Animation(R"(../Animations/Hero/standAfterDuck)");
	show(animation, topLeft);
	animation = Animation(R"(../Animations/Hero/idle)");
	key = cv::waitKey(100);
	while (key == -1) {
		show(animation, topLeft);
		key = cv::waitKey(100);
		if (key > 0) {
			return key;
		}
	}
}

int jumpRight(cv::Point& topLeft) {

	Animation animation(R"(../Animations/Hero/jump)");
	show(animation, topLeft, 20, 30);
	animation = Animation(R"(../Animations/Hero/idle)");
	int key = cv::waitKey(100);
	while (key == -1) {
		show(animation, topLeft);
		key = cv::waitKey(100);
		if (key > 0) {
			return key;
		}
	}
}
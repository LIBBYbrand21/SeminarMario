#include "Animation.h"
#include <opencv2/opencv.hpp>

#include "SlimeEntity.h"
#include "LiveEntity.h"
#include "vector"
#include "ScoreEntity.h"
using namespace cv;

#define HERO_DUCK 's'
#define HERO_JUMP 'w'
#define HERO_STAND_AFTER_DUCK 'w'
#define HERO_RUN_RIGHT 'd'
#define HERO_IDLE 'a'

Mat background = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
auto slime = CreateSlimeEnemy(R"(../Animations/SlimeOrange)");
auto live1 = CreateLive();
auto live2 = CreateLive();
auto live3 = CreateLive();
auto score = CreateScore();
bool isToExit = false;


int walkRight(Point& topLeft);
int idle(Point& topLeft);
int duck(Point& topLeft);
int jumpRight(Point& topLeft);


int main()
{
	resize(background, background, cv::Size(), 0.79, 0.75);
	Point topLeft(background.size().width, background.size().height * 2.03 / 3);

	slime->reset(Point(background.size().width * 2 / 3, background.size().height * 4 / 5));
	live1->reset(Point(background.size().width * 9.5 / 10,/* background.size().height / 22*/30));
	live2->reset(Point(background.size().width * 9 / 10, /* background.size().height / 22*/30));
	live3->reset(Point(background.size().width * 8.5 / 10, /* background.size().height / 22*/30));
	score->reset(Point(15,50));

	int key = idle(topLeft);

	while (false == isToExit)
	{
		switch (key)
		{
		case 27: { isToExit = true; break; }
		case HERO_DUCK: {key = duck(topLeft); break; }
		case HERO_JUMP: {key = jumpRight(topLeft); break; }
		case HERO_RUN_RIGHT: {key = walkRight(topLeft); break; }
		case HERO_IDLE: {key = idle(topLeft); break; }
			default:
				key = waitKey(100);
		}
	}


	return 0;
}
void show(Animation& animation, Point& topLeft, int x = 0, int y = 0) {
	for (int i = 0; i < animation.numFrames(); i++)
	{
		Frame const& frame = animation.getFrame(i);
		Mat canvas = background.clone();
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

		// @2: slime is an "EntityState" and knows how to take care of itself:
		slime->update();
		slime->draw(canvas); 
		live1->update();
		live1->draw(canvas);
		live2->update();
		live2->draw(canvas);
		live3->update();
		live3->draw(canvas);
		score->update();
		score->draw(canvas);


		imshow("test", canvas);
		int key = waitKey(100);
	}
}

int walkRight(Point& topLeft) {
	int x=-1;
	Animation animation(R"(../Animations/Hero/runRight)");
	int key = waitKey(100);
	while (key == -1) {
		show(animation, topLeft, background.size().width / 10 / animation.numFrames());
		int key = waitKey(100);
		if (key > 0) {
			x = key;
			break;
		}
	}
	return x;
}

int idle(Point& topLeft) {
	int x = -1;
	Animation animation(R"(../Animations/Hero/idle)");
	int key = waitKey(100);
	while (key == -1) {
		show(animation, topLeft);
		key = waitKey(100);
		if (key > 0) {
			x = key;
			break;
		}
	}
	return x;
}

int duck(Point& topLeft)
{
	int x;
	Animation animation(R"(../Animations/Hero/duckDown)");
	show(animation, topLeft);
	animation = Animation(R"(../Animations/Hero/duckStay)");
	int key = waitKey(100);
	while (key != HERO_STAND_AFTER_DUCK) {
		Frame const& frame = animation.getFrame(0);
		Mat canvas = background.clone();
		drawFrame(frame, canvas, topLeft);
		slime->update();
		slime->draw(canvas);
		imshow("test", canvas);
		int key = waitKey(100);
		if (key == HERO_STAND_AFTER_DUCK) {
			x = key;
			break;
		}
		if (key == 27) {
			x = key;
			isToExit = true;
			return x;
		}
	}
	animation = Animation(R"(../Animations/Hero/standAfterDuck)");
	show(animation, topLeft);
	return idle(topLeft);
}

int jumpRight(Point& topLeft) {

	Animation animation(R"(../Animations/Hero/jump)");
	show(animation, topLeft, 20, 30);
	return idle(topLeft);
}
#include "Animation.h"
#include <opencv2/opencv.hpp>
#include "Timer.h"
#include "HeroEntity.h"

// @2: include the slime enemy:
#include "SlimeEntity.h"
#include "LiveEntity.h"
#include "ScoreEntity.h"
#include "EntitiesPool.h"
#include "AppleEntity.h"
// @2: no need to write cv:: every time, we can just use namespace:
using namespace cv;
// @2: NOTE! you should "use namespace" only in cpp file, NOT in H files! (because it "passes on" with the #include statement.)


// before you start: open SeminarMario project properties, go to 
// Debugging -> Environment
// set the following value:
// PATH=$(PATH);../OpenCV_451/bin
// hit Ctrl + F5 and see a walking lady. that's our hero!
// press Esc to exit.
// read carefully the comments below - it contains explanations and instructions.
// and do the exercises.

int main()
{
	// OpenCV is a popular image processing and computer vision library.
	// it is not part of the C++ language, but it's written in C++.
	// in this project we integrated it for you.
	// The class that represents an image in OpenCV is Mat (i.e. Matrix),
	// and it's defined inside namespace cv. So the full name is Mat.
	Mat background = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
	resize(background, background, cv::Size(), 0.79, 0.75);
	
	EntityPtr live = createLive(R"(../Animations/heart.png)");
	live->reset(Point(background.size().width * 0.99, background.size().height * 0.03));

	EntityPtr score = CreateScore();
	score->reset(Point(background.size().width * 0.01, background.size().height * 0.05));
	
	EntityPtr hero = createHero(R"(../Animations/Hero)");
	hero->reset(Point(background.size().width / 2, background.size().height * 2.03 / 3));

	EntityPtr apple = createApple(R"(../Animations/apple2.png)");
	apple->reset(Point(background.size().width / 2+70, background.size().height*4.4/6));

	EntitiesPoolPtr slimePool= createSlimePool(R"(../Animations/SlimeOrange)",5);
	EntityPtr slime = slimePool->getNext();
	slime->reset(Point(background.size().width * 2 / 3, background.size().height * 4 / 5));


	Timer timer(/*freq. ms=*/100);
	hero->Register(live);
	hero->Register(hero);
	//hero->Register(slime);
	hero->Register(score);
	timer.Register(slime);
	timer.Register(hero);
	timer.Register(apple);

	bool isToExit = false;
	while (false == isToExit)
	{
		Mat canvas = background.clone();

		timer.tick();
		slime->draw(canvas);
		hero->draw(canvas);
		hero->checkCollision(slime);
		live->draw(canvas);
		score->draw(canvas);
		apple->draw(canvas);

		imshow("test", canvas);
//		slime->checkCollision(hero);

	}

	return 0;
}

// 1.
// draws an animation of our hero that ducks in place, and stays down till you 
// press the 'w' key. (if (key == 'w'))
void duck();

// 2.
// draws an animation of our hero that walks to the right, and ACTUALLLY MOVES RIGHT
// with a constant speed, of (1/10)*(image width) per 100 milliseconds (=0.1 second).
void walkRight();

// 3.
// draw our hero jumps up and right, and stays standing after jump finishes.
void jumpRight();

// 4.
// write a main() that:
// when user presses 'd' - hero walks right
// when user presses 's' - hero ducks
// when user presses 'w' - hero jumps
// when user presses 'a' - hero stops (if it was walking right before)

// 5.
// in walkRight - when hero reaches the canvas boundery - don't let it go outside of image.
// force the hero x coordinate to be such that we'll see her.
// hint: you can get hero image Mat with frame.image

// 6.
// do the same with jumpRight
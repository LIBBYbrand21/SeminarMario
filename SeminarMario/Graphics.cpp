#include "Graphics.h"
#include <algorithm>

using namespace cv;
using namespace std;

//////////////////////////////////////////////////
//SingleAnimationGraphics
SingleAnimationGraphics::SingleAnimationGraphics(AnimationPtr animPtr, bool isCyclic)
	:_animation(animPtr), _currFrameIdx(0)
	, _isCyclic(isCyclic)
{
}
Mat SingleAnimationGraphics::getCollisionMask()
{
	return _animation->getFrame(_currFrameIdx).mask;
}
void SingleAnimationGraphics::draw(cv::Mat& canvas, cv::Point const& topLeft)
{
	drawFrame(_animation->getFrame(_currFrameIdx), canvas, topLeft);
}
void SingleAnimationGraphics::reset(int code)
{
	_currFrameIdx = 0;
}
bool SingleAnimationGraphics::update()
{
	bool hasFinished = (_currFrameIdx == _animation->numFrames() - 1);
	if (_isCyclic)
		_currFrameIdx = (_currFrameIdx + 1) % _animation->numFrames();
	else
		_currFrameIdx = min<int>(_currFrameIdx + 1, (int)_animation->numFrames() - 1);

	return hasFinished;
}

//LivesGraphics
LivesGraphics::LivesGraphics(string const& path)
	:_singleLife(Frame(path)), _livesCount(3)
{
	resize(_singleLife.image, _singleLife.image, Size(_singleLife.mask.size().width / 10, _singleLife.mask.size().height / 10), 0, 0);
	resize(_singleLife.mask, _singleLife.mask, Size(_singleLife.mask.size().width / 10, _singleLife.mask.size().height / 10), 0, 0);
}
Mat LivesGraphics::getCollisionMask()
{
	return _singleLife.mask;
}
void LivesGraphics::draw(cv::Mat& canvas, cv::Point const& topLeft)
{
	Point curr_topleft = topLeft;
	for (int i = 0; i < _livesCount; i++)
	{
		curr_topleft.x -= _singleLife.image.cols * 0.9;
		drawFrame(_singleLife, canvas, curr_topleft);
	}
	//drawFrame(_singleLife, canvas, topLeft);
}
void LivesGraphics::reset(int code)
{
	_livesCount = 3;
}
bool LivesGraphics::update()
{
	if (_livesCount > 0) {
		_livesCount--;
	}
	return false;
}

//ScoresGraphics
ScoresGraphics::ScoresGraphics(float fontScale, int fontFace)
	:_fontScale(fontScale), _fontFace(fontFace), _score(0)
{
}
Mat ScoresGraphics::getCollisionMask()
{
	return Mat();
}
void  ScoresGraphics::draw(cv::Mat& canvas, cv::Point const& topLeft)
{
	string text = "score: ";
	string moreText = to_string(_score);
	text = text + moreText;
	Scalar color(0, 0, 255);
	int thickness = 2;
	int lineType = FILLED;
	putText(canvas, text, topLeft, _fontFace, _fontScale, color, thickness, lineType);
	imshow("test", canvas);
}
void ScoresGraphics::reset(int code)
{
	_score = code;
}
bool ScoresGraphics::update()
{
	_score += 10;
	return true;
}

EmptyGraphicsDecorator::EmptyGraphicsDecorator()
{
}
cv::Mat EmptyGraphicsDecorator::getCollisionMask()
{
	return Mat();
}
void EmptyGraphicsDecorator::draw(cv::Mat& canvas, cv::Point const& topLeft)
{
	return;
}
void EmptyGraphicsDecorator::reset(int code)
{
	return;
}
bool EmptyGraphicsDecorator::update()
{
	return true;
}

//////////////////////////////////////////////////

void exampleOfPutText()
{
	Mat image = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
	string text = "score: ";
	int score = 465;
	string moreText = to_string(score);
	text = text + moreText;
	Point topLeft(50, 50);
	int fontFace = FONT_HERSHEY_SIMPLEX; // normal size sans - serif font
	// other options for fontFace:
	FONT_HERSHEY_PLAIN;// small size sans - serif font
	FONT_HERSHEY_DUPLEX;// normal size sans - serif font(more complex than FONT_HERSHEY_SIMPLEX)
	FONT_HERSHEY_COMPLEX; // normal size serif font
	FONT_HERSHEY_TRIPLEX; //normal size serif font(more complex than FONT_HERSHEY_COMPLEX)
	FONT_HERSHEY_COMPLEX_SMALL; //smaller version of FONT_HERSHEY_COMPLEX
	FONT_HERSHEY_SCRIPT_SIMPLEX; //hand - writing style font
	FONT_HERSHEY_SCRIPT_COMPLEX;// more complex variant of FONT_HERSHEY_SCRIPT_SIMPLEX
	double fontScale = 2; // Font scale factor that is multiplied by the font-specific base size.
	Scalar color(/*Blue between 0...255 =*/0,/*Green between 0...255 =*/0,/*Red between 0...255 =*/255);
	int thickness = 2;
	int lineType = FILLED;
	// other options for line type:
	LINE_4; LINE_8; LINE_AA;

	// finally:
	putText(image, text, topLeft, fontFace, fontScale, color, thickness, lineType);

	imshow("test", image);
	waitKey();//waiting forever till some key is hit.
}

AppleGraphics::AppleGraphics(std::string const& path, bool isActive)
	:_singleApple(Frame(path)), _isActive(isActive)
{
	resize(_singleApple.image, _singleApple.image, Size(_singleApple.mask.size().width / 8, _singleApple.mask.size().height / 8), 0, 0);
	resize(_singleApple.mask, _singleApple.mask, Size(_singleApple.mask.size().width / 8, _singleApple.mask.size().height / 8), 0, 0);

}
Mat AppleGraphics::getCollisionMask()
{
	return _singleApple.mask;
}
void AppleGraphics::draw(cv::Mat& canvas, cv::Point const& topLeft)
{
	if (_isActive)
		drawFrame(_singleApple, canvas, topLeft);
}
void AppleGraphics::reset(int code)
{
	//_isActive = false;
}
bool AppleGraphics::update()
{
	return false;
}

FlickeringDecorator::FlickeringDecorator(IGraphicsComponentPtr base)
	:_base(base), i(0)
{
}
cv::Mat FlickeringDecorator::getCollisionMask()
{
	return _base->getCollisionMask();
}
void FlickeringDecorator::draw(cv::Mat& canvas, cv::Point const& topLeft)
{
	if (i % 2 == 0)
		_base->draw(canvas, topLeft);
	if (i > 20)i = 0;
	else i++;
}
void FlickeringDecorator::reset(int code)
{
	i = 0;
	_base->reset();
}
bool FlickeringDecorator::update()
{
	if (i == 20)return true;
	return _base->update();
}

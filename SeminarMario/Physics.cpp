#include "Physics.h"
#include <algorithm>

using namespace cv;
using namespace std;
/// //////////////////////////////////////////

// @2: Read this function and try to understand what it does. Use Image Watch !
bool checkPixelLevelCollision(IPhysicsComponent const* first, IPhysicsComponentPtr const& other)
{
	cv::Mat const& mask_First = first->getCollisionMask();
	cv::Mat const& mask_Other = other->getCollisionMask();
	if (mask_Other.empty() || mask_First.empty())
		return false;

	Point TL_first = first->getTL();
	Point TL_other = other->getTL();
	
	// ROI = Reagion of Interest
	Rect firstROI(TL_first, TL_first + Point(mask_First.size()));
	Rect othersROI(TL_other, TL_other + Point(mask_Other.size()));
	Rect intersection = firstROI & othersROI;
	if (intersection.empty())
		return false;

	Rect intersection_RelativeToMe(
		intersection.tl() - TL_first, intersection.br() - TL_first);

	Rect intersection_RelativeToOther(
		intersection.tl() - TL_other, intersection.br() - TL_other);

	Mat myMaskROI = mask_First(intersection_RelativeToMe);
	Mat othersMaskROI = mask_Other(intersection_RelativeToOther);
	Mat whereMeAndOtherIntersect = myMaskROI.mul(othersMaskROI);

	return countNonZero(whereMeAndOtherIntersect);
}

/// //////////////////////////////////////////

FixedWidgetPhysics::FixedWidgetPhysics()
	:_topLeft(0,0), _mask()
{
}

void FixedWidgetPhysics::reset(cv::Point const& tl)
{
	_topLeft = tl;
}

bool FixedWidgetPhysics::update(cv::Mat const& collisionMask)
{
	_mask = collisionMask;
	return false;
}

bool FixedWidgetPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

cv::Point const& FixedWidgetPhysics::getTL() const
{
	return _topLeft;
}

cv::Mat const& FixedWidgetPhysics::getCollisionMask() const
{
	return _mask;
}

/// ////////////////////////////////////////////////////
ConstVelocityPhysics::ConstVelocityPhysics(cv::Point const& velocity)
	:_currTL(0, 0), _velocity(velocity)
{
}

void ConstVelocityPhysics::reset(cv::Point const& tl)
{
	_currTL = tl;
}

bool ConstVelocityPhysics::update(cv::Mat const& collisionMask) {
	_mask = collisionMask;
	_currTL += _velocity;
	return false;
}

Mat const& ConstVelocityPhysics::getCollisionMask() const
{
	// TODO: insert return statement here
	return _mask;
}

bool ConstVelocityPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

Point const& ConstVelocityPhysics::getTL() const
{
	return _currTL;// TODO: insert return statement here
}

NonCollidingPhysicsDecorator::NonCollidingPhysicsDecorator(IPhysicsComponentPtr base)
	:_base(base)
{
}

void NonCollidingPhysicsDecorator::reset(cv::Point const& tl)
{
	_base->reset(tl);
}

bool NonCollidingPhysicsDecorator::update(cv::Mat const& collisionMask)
{
	return _base->update(collisionMask);
}

cv::Mat const& NonCollidingPhysicsDecorator::getCollisionMask() const
{
	return Mat();
}

bool NonCollidingPhysicsDecorator::checkCollision(std::shared_ptr<IPhysicsComponent> const& other) const
{
	return false;
}

Point const& NonCollidingPhysicsDecorator::getTL() const
{
	return _base->getTL();
}

BoundedPhysicsDecorator::BoundedPhysicsDecorator(IPhysicsComponentPtr base)
	:_base(base), _bounds(Rect(0, 0, 1400, 1400)) 
{
}
void BoundedPhysicsDecorator::reset(cv::Point const& tl)
{
	_base->reset(tl);
}
bool BoundedPhysicsDecorator::update(cv::Mat const& collisionMask)
{
	test();
	return _base->update(getCollisionMask());
}
cv::Mat const& BoundedPhysicsDecorator::getCollisionMask() const
{
	return _base->getCollisionMask();
	// TODO: insert return statement here
}
bool BoundedPhysicsDecorator::checkCollision(std::shared_ptr<IPhysicsComponent> const& other) const
{
	return _base->checkCollision(other);
}
cv::Point const& BoundedPhysicsDecorator::getTL() const
{
	return _base->getTL();
	// TODO: insert return statement here
}
void BoundedPhysicsDecorator::test()
{

	bool ifContains = _bounds.contains(Point(_base->getTL().x, _base->getTL().x));
	if (!ifContains)
		_base->reset(Point(0, _base->getTL().y));
}

JumpPhysics::JumpPhysics(int horizontalVelocity, int initialVerticalVelocity, int gravity)
{
	this->_initialJumpVelocity.x = horizontalVelocity;
	this->_initialJumpVelocity.y = initialVerticalVelocity;
	this->_gravity = gravity;
	this->_currVelocity.x = horizontalVelocity;
	this->_currVelocity.y = initialVerticalVelocity;
}

void JumpPhysics::reset(cv::Point const& tl)
{
	//in the middle of the jump , sould not reset the jump startTL

	_jumpStartTL = tl;
	_currVelocity = _initialJumpVelocity;
	_currTL = tl;
}

bool JumpPhysics::update(cv::Mat const& collisionMask)
{
	_mask = collisionMask;
	_currTL.x += _currVelocity.x;
	_currTL.y -= _currVelocity.y;
	_currVelocity.y -= _gravity;
	cout << _currVelocity.y << endl;
	return  _currTL.y == _jumpStartTL.y;
}

Mat const& JumpPhysics::getCollisionMask() const
{
	return this->_mask;
}

bool JumpPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

Point const& JumpPhysics::getTL() const
{
	return this->_currTL;
}
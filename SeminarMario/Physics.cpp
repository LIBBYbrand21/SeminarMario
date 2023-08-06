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
	this->_mask = collisionMask;
	this->_currTL.x += this->_velocity.x;
	this->_currTL.y += this->_velocity.y;
	return true;
}

cv::Mat const& ConstVelocityPhysics::getCollisionMask() const
{
	// TODO: insert return statement here
	return _mask;
}

bool ConstVelocityPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

cv::Point const& ConstVelocityPhysics::getTL() const
{
	return _currTL;// TODO: insert return statement here
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

	this->_jumpStartTL = tl;
	this->_currVelocity.y = this->_initialJumpVelocity.y;
	this->_currVelocity.x = this->_initialJumpVelocity.x;
	this->_currTL = tl;

}

bool JumpPhysics::update(cv::Mat const& collisionMask)
{
	this->_mask = collisionMask;
	_currTL.x += +_currVelocity.x;
	_currTL.y -= _currVelocity.y;
	_currVelocity.y -= _gravity;
	return  this->_currTL.y == this->_jumpStartTL.y;
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
#include "Entities.h"
#include "Graphics.h"
#include "Physics.h"
#include "Animation.h"

#include <memory>
using namespace std;
using namespace cv;

EntityState::EntityState(IGraphicsComponentPtr graphics, IPhysicsComponentPtr physics)
	:_graphicsPtr(graphics), _physicsPtr(physics)
{
}

void EntityState::update()
{
	bool graphicsFinished = _graphicsPtr->update();
	bool physicsFinished = _physicsPtr->update(_graphicsPtr->getCollisionMask());
}

IPhysicsComponentPtr const& EntityState::getPhysics() const
{
	return _physicsPtr;
}

void EntityState::reset(cv::Point const& TL)
{
	_graphicsPtr->reset();
	_physicsPtr->reset(TL);
}

void EntityState::draw(cv::Mat & canvas)
{
	_graphicsPtr->draw(canvas, _physicsPtr->getTL());
}
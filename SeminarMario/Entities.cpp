#include "Entities.h"
#include "Graphics.h"
#include "Physics.h"
#include "Animation.h"
#include "Config.h"

#include <memory>
#include "Timer.h"
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

	if (physicsFinished)
		Notify(Event{EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::ENTITY_PHYSICS_FINISHED});
	if (graphicsFinished) {
		// @4 TODO.
		Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION });
	}
}

void EntityState::addState(Event const& e, std::shared_ptr<EntityState> toWhere)
{
	_adjecentStates[e] = toWhere;
}

shared_ptr<EntityState> EntityState::tryModifyState(Event const& e) const
{
	bool isEventMapped = _adjecentStates.count(e);
	if (isEventMapped)
		return _adjecentStates.at(e);
	
	return nullptr;
}

IPhysicsComponentPtr const& EntityState::getPhysics() const
{
	return _physicsPtr;
}

void EntityState::setPhysics(IPhysicsComponentPtr physics)
{
	_physicsPtr = physics;
}

IGraphicsComponentPtr const& EntityState::getGraphics() const
{
	return _graphicsPtr;
}

void EntityState::setGraphics(IGraphicsComponentPtr graphics)
{
	_graphicsPtr = graphics;
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


////////////////////////////////////////////////////////

Entity::Entity(EntityStatePtr state)
	:_state(state)
{
}

void Entity::onNotify(Event const& e)
{
	if (e.sender == EventSenders::SENDER_TIMER
		&&
		e.type == EventTypes::EVENT_TIMER
		&&
		e.code == EventCodes::TIME_TICK ||

		e.sender == EventSenders::SENDER_ENTITY_STATE
		&&
		e.type == EventTypes::EVENT_PHYSICS
		&&
		e.code == EventCodes::COLLISION_WITH_ENEMY)
	{
		_state->update();
	}
	if (e.sender == EventSenders::SENDER_TIMER
		&&
		e.type == EventTypes::EVENT_TIMER
		&&
		e.code == EventCodes::TIME_FLICKERING_FINISH)
	{

		//_state->setPhysics();
		FlickeringDecorator* graphicsPtr = dynamic_cast<FlickeringDecorator*>(_state->getGraphics().get());
		if (graphicsPtr != nullptr) {
			_state->setPhysics(dynamic_cast<NonCollidingPhysicsDecorator*>(_state->getPhysics().get())->getBase());
			_state->setGraphics(graphicsPtr->getBase());
		}
	}
	
	auto newStateCandidate = _state->tryModifyState(e);
	
	if (newStateCandidate)
	{
		newStateCandidate->reset(_state->getPhysics()->getTL());
		_state = newStateCandidate;
	}
}

void Entity::reset(cv::Point const& TL)
{
	_state->reset(TL);
}

void Entity::draw(cv::Mat& canvas)
{
	_state->draw(canvas);
}

void Entity::checkCollision(EntityPtr& other) {
	if (this->_state->getPhysics()->checkCollision(other->_state->getPhysics())) {
		Notify(Event{ EventSenders::SENDER_ENTITY_STATE,EventTypes::EVENT_PHYSICS,EventCodes::COLLISION_WITH_ENEMY });
		_state->setPhysics(make_shared<NonCollidingPhysicsDecorator>(_state->getPhysics()));
		_state->setGraphics(make_shared<FlickeringDecorator>(_state->getGraphics()));
		TimerPtr timer = Timer::GetInstance(100);
		timer->setTimeToFlicering();
	}
}
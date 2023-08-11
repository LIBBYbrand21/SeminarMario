#include "AppleEntity.h"
#include "Config.h"

using namespace cv;
using namespace std;

EntityStatePtr CreateAppleState(std::string const& path, bool isActive)
{
	IGraphicsComponentPtr graphicsPtr
	/*=make_shared<EmptyGraphicsDecorator>*/(
		new AppleGraphics(path, isActive));
	IPhysicsComponentPtr physicsPtr(new JumpPhysics(10,42,7));

	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}

EntityPtr createApple(std::string const& path)
{
	EntityStatePtr appleActive = CreateAppleState(path, true);
	EntityStatePtr appleNotActive = CreateAppleState(path, false);
	EntityPtr apple(new Entity(appleNotActive));
	appleActive->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_FOOD_LIFE }, appleNotActive);
	appleActive->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::ENTITY_PHYSICS_FINISHED }, appleNotActive);
	appleNotActive->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_SHOOT }, appleActive);
	appleActive->Register(apple);
	appleNotActive->Register(apple);
	return apple;
}

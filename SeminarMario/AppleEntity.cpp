#include "AppleEntity.h"

using namespace cv;
using namespace std;

EntityStatePtr CreateAppleState(std::string const& path)
{
	IGraphicsComponentPtr graphicsPtr
		/*=make_shared<EmptyGraphicsDecorator>*/(
		new AppleGraphics(path));
	IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();
	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}

EntityPtr createApple(std::string const& path)
{
	auto apple = CreateAppleState(path);
	EntityPtr appleEntity(new Entity(apple));
	return appleEntity;
}

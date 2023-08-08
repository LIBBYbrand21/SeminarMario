#include "LiveEntity.h"

using namespace cv;
using namespace std;

EntityStatePtr CreateLiveState(std::string const& path)
{
	IGraphicsComponentPtr graphicsPtr(
		new LivesGraphics(path));
	IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();
	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}

EntityPtr createLive(std::string const& path)
{
	auto live = CreateLiveState(path);
	EntityPtr liveEntity(new Entity(live));
	return liveEntity;
}

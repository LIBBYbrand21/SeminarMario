#include "LiveEntity.h"

using namespace cv;
using namespace std;

EntityStatePtr CreateLiveState(std::string const& animationFolder)
{
	IGraphicsComponentPtr graphicsPtr(
		new LivesGraphics(animationFolder));
	IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();
	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}

EntityPtr createLive(std::string const& animationFolder)
{
	auto live = CreateLiveState(animationFolder);
	EntityPtr liveEntity(new Entity(live));
	return liveEntity;
}

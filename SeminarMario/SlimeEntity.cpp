#include "SlimeEntity.h"

using namespace cv;
using namespace std;

EntityStatePtr CreateSlimeEnemy(std::string const& animationFolder)
{
	AnimationPtr animation(new Animation(animationFolder));
	bool isCyclic = true;

	IGraphicsComponentPtr graphicsPtr(
		new SingleAnimationGraphics(animation, isCyclic));

	IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();

	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}
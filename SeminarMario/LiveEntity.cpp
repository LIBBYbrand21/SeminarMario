#include "SlimeEntity.h"

using namespace cv;
using namespace std;

EntityStatePtr CreateHeart()
{
	string path = (R"(../Animations/Live/heart.png)");
	IGraphicsComponentPtr graphicsPtr(
		new LivesGraphics(path));
	IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();
	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}
#include "LiveEntity.h"

using namespace cv;
using namespace std;

EntityStatePtr CreateLive()
{
	string path = (R"(../Animations/Live/heart.png)");
	IGraphicsComponentPtr graphicsPtr(
		new LivesGraphics(path));
	IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();
	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}
#include "ScoreEntity.h"

using namespace cv;
using namespace std;

EntityStatePtr CreateScore()
{
	IGraphicsComponentPtr graphicsPtr(
		new ScoresGraphics(/*FONT_HERSHEY_SIMPLEX FONT_HERSHEY_COMPLEX,*/ FONT_HERSHEY_DUPLEX, 0.5));
	IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();
	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}
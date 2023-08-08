#include "ScoreEntity.h"
#include "Config.h"

using namespace cv;
using namespace std;

EntityStatePtr createScoreState()
{
	IGraphicsComponentPtr graphicsPtr(
		new ScoresGraphics(/*FONT_HERSHEY_SIMPLEX FONT_HERSHEY_COMPLEX,*/ FONT_HERSHEY_DUPLEX, 5));
	IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();
	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}

EntityPtr CreateScore()
{
	auto scoreState = createScoreState();
	EntityPtr score(new Entity(scoreState));
	return score;
}

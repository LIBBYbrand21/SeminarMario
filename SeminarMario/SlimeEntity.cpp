#include "SlimeEntity.h"
#include <memory>
#include <filesystem>
namespace fs = std::filesystem;

using namespace cv;
using namespace std;

EntityStatePtr CreateSlimeEnemy(std::string const& animationFolder)
{
	AnimationPtr animation(new Animation(animationFolder));
	bool isCyclic = true;

	IGraphicsComponentPtr graphicsPtr(
		new SingleAnimationGraphics(animation, isCyclic));

	IPhysicsComponentPtr nonCollidingPhysicsPtr =
		make_shared<NonCollidingPhysicsDecorator>(
			make_shared<FixedWidgetPhysics>());

	return make_shared<EntityState>(graphicsPtr, nonCollidingPhysicsPtr);
}

EntityPtr createSlime(std::string const& animationFolder)
{
	auto slime = CreateSlimeEnemy(animationFolder);
	EntityPtr slimeEntity(new Entity(slime));
	return slimeEntity;
}


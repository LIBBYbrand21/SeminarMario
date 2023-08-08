#include "SlimeEntity.h"
#include <memory>
#include <filesystem>
#include "Config.h"
#include "EntitiesPool.h"

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


EntitiesPoolPtr createSlimePool(std::string const& animationFolder, int slimes) {
	EntitiesPoolPtr pool(new EntitiesPool);
	for (int i = 0; i < slimes; i++) {
		pool->insert(createSlime(animationFolder));
	}
	return pool;
}


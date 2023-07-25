#pragma once
#include "Graphics.h"
#include "Physics.h"

class EntityState
{
protected:
	IGraphicsComponentPtr _graphicsPtr;
	IPhysicsComponentPtr _physicsPtr;

public:
	EntityState(IGraphicsComponentPtr graphics, IPhysicsComponentPtr physics);

	virtual void update();

	IPhysicsComponentPtr const& getPhysics() const;
	virtual void reset(cv::Point const& TL);

	void draw(cv::Mat & canvas);
};
typedef std::shared_ptr<EntityState> EntityStatePtr;
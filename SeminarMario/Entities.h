#pragma once
#include "Graphics.h"
#include "Physics.h"
#include "Observer.h"
#include <map>

class EntityState : public Subject
{
protected:
	IGraphicsComponentPtr _graphicsPtr;
	IPhysicsComponentPtr _physicsPtr;

	std::map<Event, std::shared_ptr<EntityState>> _adjecentStates;


public:
	EntityState(IGraphicsComponentPtr graphics, IPhysicsComponentPtr physics);

	virtual void update();
	
	void addState(Event const& e, std::shared_ptr<EntityState> toWhere);
	virtual std::shared_ptr<EntityState> tryModifyState(Event const& e) const;

	IPhysicsComponentPtr const& getPhysics() const;
	void setPhysics(IPhysicsComponentPtr physics); 
	IGraphicsComponentPtr const& getGraphics() const;
	void setGraphics(IGraphicsComponentPtr graphics);
	virtual void reset(cv::Point const& TL);

	void draw(cv::Mat & canvas);
};
typedef std::shared_ptr<EntityState> EntityStatePtr;


class Entity : public IObserver,public Subject
{
protected:
	EntityStatePtr _state;

public:
	Entity(EntityStatePtr state);
	typedef std::shared_ptr<Entity> EntityPtr;

	// Inherited via IObserver
	void checkCollision(EntityPtr& other);
	virtual void onNotify(Event const& e) override;
	virtual void reset(cv::Point const& TL);
	void draw(cv::Mat& canvas);

	// Prototype Design Pattern
	//virtual std::shared_ptr<Entity> clone() = 0;
};
typedef std::shared_ptr<Entity> EntityPtr;


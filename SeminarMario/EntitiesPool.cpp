#include "EntitiesPool.h"

EntitiesPool::EntitiesPool()
{
}

void EntitiesPool::insert(EntityPtr whom)
{
	_pool.push_back(whom);
	_isInUse.push_back(false);
}

EntityPtr EntitiesPool::getNext()
{
	for (int i = 0; i < _pool.size(); i++) {
		if (!_isInUse[i]) {
			_isInUse[i] = true;
			return _pool[i];
		}
	}
	throw std::exception("all entities in use now!!");
}

void EntitiesPool::returnEntity(EntityPtr toFree)
{
	for (int i = 0; i < _pool.size(); i++) {
		if (_isInUse[i] && _pool[i] == toFree) {
			_isInUse[i] = false;
			return;
		}
	}
}

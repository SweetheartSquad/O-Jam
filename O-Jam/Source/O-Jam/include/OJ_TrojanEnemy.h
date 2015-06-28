#pragma once 

#include <OJ_Enemy.h>
#include <OJ_Arena.h>
#include <Timeout.h>

class OJ_TrojanEnemy : public OJ_Enemy {
public:

	Timeout * spawnTimer;
	OJ_Arena * arena;

	OJ_TrojanEnemy(Box2DWorld * _world, OJ_Arena * _arena);
	~OJ_TrojanEnemy();

	void update(Step * _step) override;
};

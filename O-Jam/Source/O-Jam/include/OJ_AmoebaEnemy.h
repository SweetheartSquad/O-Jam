#pragma once


#include <OJ_Enemy.h>
#include <OJ_Arena.h>
#include <Timeout.h>

class OJ_AmoebaEnemy : public OJ_Enemy {
public:

	Timeout * spawnTimer;
	OJ_Arena * arena;

	OJ_AmoebaEnemy(Box2DWorld * _world, OJ_Arena * _arena, float _componentmult);
	~OJ_AmoebaEnemy();

	void update(Step * _step) override;
};

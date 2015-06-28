#pragma once

#include <OJ_Enemy.h>

class OJ_DdosEnemy : public OJ_Enemy {
public:	
	OJ_DdosEnemy(Box2DWorld * _world, float _componentmult);
	
};
#pragma once

#include <Box2DWorld.h>
#include <Box2DSuperSprite.h>

class OJ_Player : public Box2DSuperSprite {
public:
	explicit OJ_Player(Box2DWorld * _world);
	
	~OJ_Player();

};
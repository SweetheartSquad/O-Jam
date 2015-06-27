#pragma once

#include <Box2dSprite.h>

class Player : public Box2DSprite {
	
	explicit Player(Box2DWorld * _world);
	
	~Player();

};
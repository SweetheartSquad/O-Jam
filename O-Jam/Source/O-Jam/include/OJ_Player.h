#pragma once

#include <Box2DWorld.h>
#include <Box2DSprite.h>
#include <TextureSampler.h>
#include <OJ_Boxer.h>

class OJ_Player : public OJ_Boxer {
public:

	explicit OJ_Player(OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = -1);
	~OJ_Player();

	void update(Step * _step) override;

	
	// move the player
	// _v should be normalized in most cases, and the player speed, mass, etc will be taken into account here
	void move(glm::vec2 _v);
	
};
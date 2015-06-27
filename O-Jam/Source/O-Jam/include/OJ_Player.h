#pragma once

#include <Box2DWorld.h>
#include <Box2DSprite.h>
#include <TextureSampler.h>
#include <OJ_Boxer.h>

class OJ_Player : public OJ_Boxer {
public:
	Box2DSprite * torso;
	Box2DSprite * handR;
	Box2DSprite * handL;

	b2PrismaticJoint * leftHandJoint;
	b2PrismaticJoint * rightHandJoint;

	explicit OJ_Player(float _componentScale, OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = -1);
	~OJ_Player();

	void update(Step * _step) override;
	virtual void move(glm::vec2 _v) override;
};
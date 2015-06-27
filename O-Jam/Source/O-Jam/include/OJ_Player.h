#pragma once

#include <Box2DWorld.h>
#include <Box2DSuperSprite.h>
#include <Box2DSprite.h>
#include <TextureSampler.h>

class OJ_TexturePack : public Node{
public:
	Texture * torsoTex;
	Texture * handTex;

	OJ_TexturePack(std::string _torsoSrc, std::string _handSrc);
};

class OJ_Player : public Box2DSuperSprite {
public:
	Box2DSprite * torso;
	Box2DSprite * handR;
	Box2DSprite * handL;

	b2PrismaticJoint * leftHandJoint;
	b2PrismaticJoint * rightHandJoint;

	float speed;

	explicit OJ_Player(OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = -1);
	~OJ_Player();

	void update(Step * _step) override;

	float punchAngle;
	float punchSpeed;
	unsigned long int punchDelay;
	float punchReach;
	
	void punchL();
	void punchR();
	// move the player
	// _v should be normalized in most cases, and the player speed, mass, etc will be taken into account here
	void move(glm::vec2 _v);
};
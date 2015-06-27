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

	b2DistanceJoint * leftHandJoint;
	b2DistanceJoint * rightHandJoint;

	float speed;

	explicit OJ_Player(OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = -1);
	~OJ_Player();

	void update(Step * _step) override;

	glm::vec2 punchDir;
	float punchSpeed;
	unsigned long int punchDelay;
	
	void punchL();
	void punchR();
	// move the player
	// _v should be normalized in most cases, and the player speed, mass, etc will be taken into account here
	void move(glm::vec2 _v);
	
private:
	int ticksSincePunchL;
	bool punchedL;
	int ticksSincePunchR;
	bool punchedR;
};
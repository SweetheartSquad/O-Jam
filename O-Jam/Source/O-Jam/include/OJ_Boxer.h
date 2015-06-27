#pragma once

#include <OJ_Character.h>

class OJ_Boxer : public OJ_Character{
public:

	Box2DSprite * handR;
	Box2DSprite * handL;

	b2DistanceJoint * leftHandJoint;
	b2DistanceJoint * rightHandJoint;

	glm::vec2 punchDir;
	float punchSpeed;
	unsigned long int punchDelay;

	OJ_Boxer(OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = -1);
	virtual ~OJ_Boxer();
	
	void update(Step * _step);

	void punchL();
	void punchR();

protected:
	int ticksSincePunchL;
	bool punchedL;
	int ticksSincePunchR;
	bool punchedR;
};

#pragma once

#include <OJ_Character.h>

class OJ_Boxer : public OJ_Character{
public:

	Box2DSprite * handR;
	Box2DSprite * handL;

	b2PrismaticJoint * handJointR;
	b2PrismaticJoint * handJointL;

	float punchAngle;
	float punchSpeed;
	float punchReach;

	OJ_Boxer(float _componentScale, OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	virtual ~OJ_Boxer();
	
	void update(Step * _step);

	void punchL();
	void punchR();
};
#pragma once

#include <OJ_Character.h>

class OJ_Enemy : public OJ_Character{
public:
	float scoreVal;

	explicit OJ_Enemy(float _componentScale, OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);

	void update(Step * step);

	void targetCharacter(OJ_Character * _target);
	void nullifyTarget();

private : 
	
	OJ_Character * target;

	void moveTowards(b2Vec2 _pos);
};
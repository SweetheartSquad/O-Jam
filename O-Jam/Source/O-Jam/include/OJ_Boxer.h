#pragma once

#include <OJ_Character.h>

class OJ_Boxer : public OJ_Character{
public:

	OJ_Boxer(OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = -1);
	virtual ~OJ_Boxer();
};

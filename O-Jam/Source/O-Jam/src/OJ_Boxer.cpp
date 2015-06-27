#pragma once

#include <OJ_Boxer.h>

OJ_Boxer::OJ_Boxer(OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	OJ_Character(_texPack, _world, _categoryBits, _maskBits, _groupIndex)
{
}

OJ_Boxer::~OJ_Boxer() {
}
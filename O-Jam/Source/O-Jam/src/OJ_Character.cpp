#pragma once

#include <OJ_Character.h>
#include <OJ_ResourceManager.h>

OJ_TexturePack::OJ_TexturePack(std::string _torsoSrc, std::string _handSrc) :
	torsoTex(OJ_ResourceManager::playthrough->getTexture(_torsoSrc)->texture),
	handTex(OJ_ResourceManager::playthrough->getTexture(_handSrc)->texture)
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


OJ_Character::OJ_Character(OJ_TexturePack* _texPack, Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	Box2DSuperSprite(_world, _categoryBits, _maskBits, _groupIndex)
{
}

OJ_Character::~OJ_Character() {
}